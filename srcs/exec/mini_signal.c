/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_signal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 20:02:54 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/26 18:29:19 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

void	here_doc_signals(int signal)
{
	t_mini	*mini_exit;

	if (signal == SIGINT)
	{
		ft_printf("\n");
		mini_exit = mem_save(NULL);
		exit_childprocess(mini_exit, 130);
	}
}

void	signal_handler(int signal)
{
	t_mini	*mini;

	if (signal == SIGINT)
	{
		ft_printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		mini = mem_save(NULL);
		mini->pipex.status = 130;
	}
}

void	more_signals(int s, struct sigaction sa)
{
	if (s == 3)
	{
		sa.sa_handler = here_doc_signals;
		sa.sa_flags = 0;
		if (sigemptyset(&sa.sa_mask) != 0)
			return ;
		sigaction(SIGINT, &sa, NULL);
		signal(SIGQUIT, SIG_IGN);
	}
}

void	signals(int s)
{
	static struct sigaction	sa;

	if (s == 1)
	{
		sa.sa_handler = signal_handler;
		sa.sa_flags = 0;
		if (sigemptyset(&sa.sa_mask) != 0)
			return ;
		sigaction(SIGINT, &sa, NULL);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (s == 2)
	{
		sa.sa_handler = SIG_DFL;
		sa.sa_flags = 0;
		if (sigemptyset(&sa.sa_mask) != 0)
			return ;
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGQUIT, &sa, NULL);
	}
	more_signals(s, sa);
}
