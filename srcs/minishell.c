/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:46:22 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/03/12 15:57:08 by dpaes-so         ###   ########.fr       */
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

void	check_built_in(char	**input)
{
	if (ft_strncpm(input, "echo", 4) == 0)
		build_echo();
	if (ft_strncmp(input, "cd", 2) == 0)
		build_cd();
	if (ft_strncmp(input, "pwd", 3) == 0)
		build_pwd();
	if (ft_strncmp(input, "export", 6) == 0)
		build_export();
	if (ft_strncmp(input, "unset", 5) == 0)
		build_unset();
	if (ft_strncmp(input, "env", 3) == 0)
		build_env();
	if (ft_strncmp(input, "exit", 4) == 0)
		build_exit(input);
}

int	main(void)
{
	char	*input;
	char *path = ft_strjoin("minishell ","");

	sig_init();
	while (1)
	{
		input = readline(path);
		if (*input)
		{
			add_history(input);
			check_built_in(&input);
			// placeholder for exit cuz i cant fucking ctrl C
			if (ft_strncmp(input, "exit", 4) == 0)
			{
				free(input);
				clear_history();
				if(path)
					free(path);
				exit(0);
			}
			if (ft_strncmp(input, "cd", 4) == 0)
			{
				char *temp = NULL;
				char *cdw = getcwd(temp,100);
				free(path);
				path = ft_strjoin("minishell >",cdw);
				free(temp);
				free(cdw);
			}
		}
		free(input);
	}
}
