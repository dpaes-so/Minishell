/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:46:22 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/05/16 18:33:58 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/mini_header.h"

void	do_here_doc(t_mini *mini, t_tree *ast, int i)
{
	if (ast->node.pipe == true)
	{
		do_here_doc(mini, ast->left, 0);
		do_here_doc(mini, ast->right, 0);
	}
	else if (ast->node.redir[i].type != T_NULL)
	{
		while(ast->node.redir[i].type != T_NULL)
		{
			ft_printf("i = %d\n",i);
			if(mini->execution_signal != 0)
				break;
			if (ast->node.redir[i].type == T_HERE_DOC)
				ast->node.here_fd = here_doc(mini->pipex, &ast->node, i,mini);
			i++;
		}
	}
}

int	main(int ac, char **av, char **ev)
{
	t_mini	mini;
	t_tree	*ast;
	char	*input;

	(void)ac;
	(void)av;
	ft_bzero(&mini, sizeof(t_mini));
	my_env_start(&mini, ev);
	get_pwd(&mini);
	mini.pipex.status = 0;
	while (1)
	{
		mini.execution_signal = 0;
		mini.pipex.path = path_finder(mini.env->my_env);
		mini.wait_check = 1;
		signals(1);
		mini.save_fd = -1;
		mini.cmd_amount = 0;
		input = readline("minishell > ");
		if (!input)
			exit_childprocess(&mini, -2);
		add_history(input);
		mini.ast = parser(input, &mini);
		ast = mini.ast;
		if (mini.ast == NULL)
			continue ;
		tree_apply_infix(mini.ast, 0, "root");
		mini.pipex.cmd = 0;
		mem_save(&mini);
		do_here_doc(&mini, ast, 0);
		if(mini.execution_signal == 0)
			run_tree(&mini, ast, 0);
		master_close();
		wait_child(&mini);
		freetrix(mini.pipex.path);
		if (mini.ast)
			free_tree(mini.ast);
		free(input);
	}
}
