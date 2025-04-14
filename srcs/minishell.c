/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:46:22 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/04/14 16:45:22 by dpaes-so         ###   ########.fr       */
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

int	check_built_in(t_mini *mini)
{
	if (ft_strncmp(mini->input, "echo ", 5) == 0)
		build_echo(mini);
	if (ft_strcmp(mini->input, "pwd") == 0)
		return (build_pwd(mini));
	if (ft_strcmp(mini->input, "env") == 0)
		return (build_env(mini));
		
	if (ft_strncmp(mini->input, "cd",2) == 0)
		return (build_cd(mini));
	if (ft_strncmp(mini->input, "export", 6) == 0)
		return(build_export(mini));
	if (ft_strncmp(mini->input, "unset", 5) == 0)
		return(build_unset(mini));
	if (ft_strcmp(mini->input, "exit") == 0)
		build_exit(mini);
	return (0);
}

void	my_env_start(t_mini *mini, char **ev)
{
	int	i;
	int	j;
	int	k;

	i = -1;
	j = 0;
	k = 0;
	mini->env = malloc(sizeof(t_env));
	while (ev[k])
		k++;
	mini->env->my_env = (char **)ft_calloc(k + 1, sizeof(char *));
	while (ev[j])
	{
		mini->env->my_env[j] = ft_strdup(ev[j]);
		j++;
	}
	while (ev[++i])
		if (ft_strnstr(ev[i], "HOME=", 5))
			break ;
	mini->env->home = ft_strdup(ev[i] + 5);
}

int	main(int ac, char **av, char **ev)
{
	t_mini	mini;
	int		pid;
	char	**matrix;
	char	*path;

	mini.pwd = NULL;
	get_pwd(&mini);
	(void)ac;
	(void)av;
	my_env_start(&mini, ev);
	sig_init();
	while (1)
	{
		mini.input = readline("Minishell >");
		if (mini.input)
		{
			add_history(mini.input);
			if (!check_built_in(&mini))
			{
				printf("I SHOULD PROB NO BE HERE IDK ASK THE GUY IN THE CHAIR\n");
				pid = fork();
				if (pid == 0)
				{
					matrix = ft_split(mini.input, ' ');
					printf("str = %s\n", mini.input);
					path = ft_strjoin("/usr/bin/", mini.input);
					execve(path, matrix, ev);
					free(path);
					free(mini.pwd);
					free(mini.env->home);
					freetrix(mini.env->my_env);
					free(mini.env);
					freetrix(matrix);
					exit(0);
				}
				wait(&pid);
			}
		}
		free(mini.input);
	}
}
