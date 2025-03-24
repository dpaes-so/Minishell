/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:46:22 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/03/24 19:44:41 by dpaes-so         ###   ########.fr       */
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

void	check_built_in(t_mini *mini)
{
	if (ft_strncmp(mini->input, "echo ", 5) == 0)
		build_echo(mini);
	if (ft_strncmp(mini->input, "cd", 2) == 0)
		build_cd(mini);
	// if (ft_strncmp(input, "pwd", 3) == 0)
	// 	build_pwd();
	// if (ft_strncmp(input, "export", 6) == 0)
	// 	build_export();
	// if (ft_strncmp(input, "unset", 5) == 0)
	// 	build_unset();
	// if (ft_strncmp(input, "env", 3) == 0)
	// 	build_env();
	if (ft_strncmp(mini->input, "exit", 4) == 0)
		build_exit(mini);
}

int	main(void)
{
	t_mini mini;
	mini.path = ft_strjoin("minishell","");

	sig_init();
	while (1)
	{
		mini.input = readline(mini.path);
		if (mini.input)
		{
			add_history(mini.input);
			check_built_in(&mini);
		}
		free(mini.input);
	}
}
