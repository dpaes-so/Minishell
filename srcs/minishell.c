/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniel <daniel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:46:22 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/06/02 00:35:17 by daniel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/mini_header.h"

void	do_here_doc(t_mini *mini, t_tree *ast, int i)
{
	if (!ast)
		return ;
	if (ast->node.pipe == true)
	{
		do_here_doc(mini, ast->left, 0);
		do_here_doc(mini, ast->right, 0);
	}
	else if (ast->node.redir && ast->node.redir[i].type != T_NULL)
	{
		while (ast->node.redir[i].type != T_NULL)
		{
			if (mini->execution_signal != 0)
				break ;
			if (ast->node.redir[i].type == T_HERE_DOC)
				ast->node.here_fd = here_doc(mini->pipex, &ast->node, i, mini);
			i++;
		}
	}
}

void	shell_execution(t_mini *mini, t_tree *ast)
{
	mini->pipex.cmd = 0;
	mem_save(mini);
	do_here_doc(mini, ast, 0);
	if (mini->execution_signal == 0)
		run_tree(mini, ast, 0);
	if (mini->f_malloc == 1)
		fmalloc(mini, "run_tree", 2);
	master_close();
	wait_child(mini);
	freetrix(mini->pipex.path);
	if (mini->ast)
	{
		free_tree(mini->ast);
		mini->ast = NULL;
	}
}

void	mini_born(t_mini *mini, char **ev)
{
	ft_bzero(mini, sizeof(t_mini));
	if (my_env_start(mini, ev) < 0)
		fmalloc(mini, "env", 2);
	get_pwd(mini);
	mini->pipex.status = 0;
	mem_save(mini);
}

void	hell_born(t_mini *mini)
{
	mini->pipex.path = NULL;
	mini->execution_signal = 0;
	if (mini->env && mini->env->my_env)
		mini->pipex.path = path_finder(mini->env->my_env, mini);
	mini->wait_check = 1;
	signals(1);
	mini->save_fd = -1;
	mini->cmd_amount = 0;
}

void	fmalloc(t_mini *mini, char *which, int code)
{
	ft_dprintf(2, "Minishell: Malloc %s: failed :(\n", which);
	clear_history();
	master_close();
	omega_free(mini);
	exit(code);
}

int	main(int ac, char **av, char **ev)
{
	t_mini	mini;
	t_tree	*ast;
	char	*input;

	(void)ac;
	(void)av;
	mini_born(&mini, ev);
	srand((unsigned int)time(NULL));
	while (1)
	{
		hell_born(&mini);
		if (mini.f_malloc == 1)
			fmalloc(&mini, "hell_born", 2);
		input = readline("Minishell > ");
		if (!input)
			exit_childprocess(&mini, -2);
		add_history(input);
		mini.ast = parser(input, &mini);
		if (mini.f_malloc == 1)
			fmalloc(&mini, "ast", 2);
		ast = mini.ast;
		if (mini.ast == NULL)
			continue ;
		tree_apply_infix(mini.ast, 0, "root");
		shell_execution(&mini, ast);
		free(input);
	}
}
