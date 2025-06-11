/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- < dgarcez-@student.42lisboa.com > +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:46:22 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/06/04 14:15:02 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/mini_header.h"

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
	get_pwd(mini);
	if (my_env_start(mini, ev) < 0)
		fmalloc(mini, "env", 2);
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

int	main(int ac, char **av, char **ev)
{
	t_mini	mini;
	t_tree	*ast;
	char	*input;

	(void)ac;
	(void)av;
	mini_born(&mini, ev);
	while (1)
	{
		hell_born(&mini);
		if (mini.f_malloc == 1)
			fmalloc(&mini, "hell_born", 2);
		input = readline("Minishell > ");
		if (!input)
		{
			ft_dprintf(2, "exit\n");
			exit_childprocess(&mini, -2);
		}
		fandre(&mini, input);
		ast = mini.ast;
		if (mini.ast == NULL)
			continue ;
		shell_execution(&mini, ast);
		free(input);
	}
}
