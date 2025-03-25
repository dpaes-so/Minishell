/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:46:22 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/03/25 19:11:16 by dpaes-so         ###   ########.fr       */
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

int	main(int ac,char **av,char **ev)
{
	t_mini mini;
	int pid;
	char **matrix;
	char *shit;
	mini.path = NULL;
	get_pwd(&mini);

	(void)ac;
	(void)av;
	sig_init();
	while (1)
	{
		mini.input = readline("Minishell >");
		if (mini.input)
		{
			add_history(mini.input);
			check_built_in(&mini);
			
			pid = fork();
			if(pid == 0)
			{
				matrix = ft_split(mini.input,' ');
				printf("str = %s\n",mini.input);
				shit = ft_strjoin("/usr/bin/",mini.input);
				execve(shit,matrix,ev);
				perror("");
				free(shit);
				free(mini.path);
				freetrix(matrix);
				exit(0);
			}
			wait(&pid);
		}
		free(mini.input);
	}
}
