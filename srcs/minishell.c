/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:46:22 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/05/05 15:03:34 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/mini_header.h"

void	handle_signal(int signal)
{
	if (signal == SIGINT)
	{
		rl_on_new_line();
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_redisplay();
	}
	return ;
}

void	sig_init(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_signal;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1)
		return (perror("Failed sigaction"));
}


int	main(int ac, char **av, char **ev)
{
	t_mini	mini;
	t_tree	*ast;
	char	*input;

	(void)ac;
	(void)av;
	sig_init();
	ft_bzero(&mini, sizeof(t_mini));
	my_env_start(&mini, ev);
	get_pwd(&mini);
	mini.pipex.path = path_finder(ev);
	while (1)
	{
		mini.save_fd = -1;
		mini.cmd_amount = 0;
		input = readline("minishell > ");
		add_history(input);
		mini.ast = parser(input, &mini);
		ast = mini.ast;
		if (mini.ast == NULL)
			continue ;
		tree_apply_infix(mini.ast, 0, "root");
		mini.pipex.cmd = 0;
		run_tree(&mini, ast, 0);
		master_close();
		wait_child(&mini);
		if (mini.ast)
			free_tree(mini.ast);
		free(input);
	}
}
