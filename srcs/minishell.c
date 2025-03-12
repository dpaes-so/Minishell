/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:46:22 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/03/12 15:36:08 by dpaes-so         ###   ########.fr       */
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
