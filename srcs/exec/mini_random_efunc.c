/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_random_efunc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:33:17 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/06/04 13:41:20 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

void	free_env(t_env *env)
{
	if (!env)
		return ;
	if (env->my_env)
		freetrix(env->my_env);
	if (env->my_export)
		freetrix(env->my_export);
	free(env);
}

int	set_shlvl(t_mini *mini)
{
	int		sh_lvl;
	char	*temp;
	char	*shlvl;
	int		i;

	i = -1;
	while (mini->env->my_env[++i])
		if (ft_strnstr(mini->env->my_env[i], "SHLVL=", 6))
			break ;
	if (!mini->env->my_env[i])
		return (fraquel(mini, 1));
	sh_lvl = ft_atoi(mini->env->my_env[i] + 6);
	sh_lvl++;
	shlvl = ft_itoa(sh_lvl);
	if (shlvl == NULL)
		return (-1);
	temp = ft_strjoin(ft_strdup("SHLVL="), shlvl);
	free(shlvl);
	if (temp == NULL)
		return (-1);
	free(mini->env->my_env[i]);
	mini->env->my_env[i] = ft_strdup(temp);
	if (mini->env->my_env[i] == NULL)
		return (-1);
	return (free(temp), 1);
}

static int	matrix_dup_env(t_mini *mini, char **ev)
{
	mini->env->my_env = ft_matrix_dup(mini->env->my_env, ev);
	if (!mini->env->my_env)
	{
		free_env(mini->env);
		mini->env = NULL;
		mini->f_malloc = 1;
		return (-1);
	}
	mini->env->my_export = ft_matrix_dup(mini->env->my_export, ev);
	if (!mini->env->my_export)
	{
		free_env(mini->env);
		mini->env = NULL;
		mini->f_malloc = 1;
		return (-1);
	}
	return (1);
}

static int	my_env_continue(t_mini *mini, char **ev)
{
	if (matrix_dup_env(mini, ev) < 0)
		return (-1);
	ft_sort_matrix(mini->env->my_export);
	if (set_shlvl(mini) < 0)
		return (set_fmalloc(mini), -1);
	return (1);
}

int	my_env_start(t_mini *mini, char **ev)
{
	int	k;

	k = 0;
	while (ev[k])
		k++;
	mini->env = ft_calloc(1, sizeof(t_env));
	if (!mini->env)
		return (mini->f_malloc = 1, -1);
	mini->env->my_env = ft_calloc(k + 1, sizeof(char *));
	if (!mini->env->my_env)
	{
		free_env(mini->env);
		mini->env = NULL;
		return (set_fmalloc(mini), -1);
	}
	mini->env->my_export = ft_calloc(k + 1, sizeof(char *));
	if (!mini->env->my_export)
	{
		free_env(mini->env);
		mini->env = NULL;
		return (set_fmalloc(mini), -1);
	}
	if (my_env_continue(mini, ev) < 0)
		return (-1);
	return (1);
}
