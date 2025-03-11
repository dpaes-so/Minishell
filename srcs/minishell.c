/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:46:22 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/03/11 19:25:37 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/mini_header.h"


void	handle_signal(int signal)
{
	if (signal == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		write(1, "\n", 1);
		write(1, "minishell >", 12);
	}
	return ;
}

int	main(void)
{
	struct sigaction	sa;
	char				*input;

	sa.sa_handler = handle_signal;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1)
		return (perror("Failed sigaction"), 1);
	while (1)
	{
		input = readline("minishell >");
		if (*input)
		{
			add_history(input);
			//placeholder for exit cuz i cant fucking ctrl C
			if (ft_strncmp(input, "exit", 4) == 0)
			{
				free(input);
				clear_history();
				exit(0);
			}
		}
		free(input);
	}
}
