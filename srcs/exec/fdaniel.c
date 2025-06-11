/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdaniel.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- < dgarcez-@student.42lisboa.com > +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:41:30 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/06/04 14:10:37 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

int	fpablo(t_mini *mini, int f, int size, char **env)
{
	env[size] = ft_strdup("SHLVL=1");
	if (!env[size])
	{
		free_env(mini->env);
		freetrix(env);
		mini->env = NULL;
		return (set_fmalloc(mini), -1);
	}
	if (f == 1)
	{
		env[++size] = ft_strdup("PWD=");
		env[size] = ft_strjoin(env[size], mini->pwd);
		if (!env[size])
		{
			free_env(mini->env);
			freetrix(env);
			mini->env = NULL;
			return (set_fmalloc(mini), -1);
		}
	}
	env[++size] = NULL;
	freetrix(mini->env->my_env);
	mini->env->my_env = env;
	return (0);
}

int	fraquel(t_mini *mini, int f)
{
	char	**env;
	int		size;

	size = -1;
	while (mini->env->my_env[++size])
		if (ft_strnstr(mini->env->my_env[size], "PWD=", 4))
			f = 0;
	env = ft_calloc(size + 2 + f, sizeof(char *));
	if (!env)
	{
		free_env(mini->env);
		mini->env = NULL;
		return (set_fmalloc(mini), -1);
	}
	env = ft_matrix_dup(env, mini->env->my_env);
	if (!env)
	{
		free_env(mini->env);
		mini->env = NULL;
		return (set_fmalloc(mini), -1);
	}
	fpablo(mini, f, size, env);
	return (21);
}

void	favila(char *s2, int fd[2], char *s, t_mini *mini)
{
	int	j;

	j = 0;
	s2 = status_expand(mini);
	if (!s2)
		return (free(s), fmalloc(mini, "here dollar ", 100));
	while (s2 && s2[j])
		write(fd[1], &s2[j++], 1);
	free(s2);
}

void	fandre(t_mini *mini, char *input)
{
	add_history(input);
	mini->ast = parser(input, mini);
	if (mini->f_malloc == 1)
		fmalloc(mini, "ast", 2);
}

void	fmalloc(t_mini *mini, char *which, int code)
{
	ft_dprintf(2, "Minishell: Malloc %s: failed :(\n", which);
	rl_clear_history();
	master_close();
	omega_free(mini);
	exit(code);
}
