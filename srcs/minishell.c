/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:46:22 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/04/23 17:34:39 by dgarcez-         ###   ########.fr       */
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
	if (ft_strncmp(mini->input, "cd", 2) == 0)
		return (build_cd(mini));
	if (ft_strncmp(mini->input, "export", 6) == 0)
		return (build_export(mini));
	if (ft_strncmp(mini->input, "unset", 5) == 0)
		return (build_unset(mini));
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

int	main(void)
{
	char	*input;

	t_tree	*tree;
	while (1)
	{
		input = readline("minishell > ");
		add_history(input);
		printf("str = %s\n", input);
		tree = parser(input);
		if (tree == NULL)
			continue;
		tree_apply_infix(tree, 0, "root");
		free_tree(tree);
		if (strcmp(input, "exit") == 0)
			exit(0);
		free(input);
	}
}
