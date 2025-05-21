/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_random_efunc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:33:17 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/20 20:04:10 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

void free_env(t_env *env)
{
    if (!env)
        return;
    if (env->my_env)
        freetrix(env->my_env);
    if (env->my_export)
        freetrix(env->my_export);
    if (env->home)
        free(env->home);
    free(env);
}


void	set_shlvl(t_mini *mini)
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
		return ;
	sh_lvl = ft_atoi(mini->env->my_env[i] + 6);
	sh_lvl++;
	shlvl = ft_itoa(sh_lvl);
	temp = ft_strjoin(ft_strdup("SHLVL="), shlvl);
	free(mini->env->my_env[i]);
	mini->env->my_env[i] = ft_strdup(temp);
	free(temp);
	free(shlvl);
}

static void my_env_continue(t_mini *mini, char **ev)
{
	int i = -1;

	mini->env->my_env = ft_matrix_dup(mini->env->my_env, ev);
	if (mini->env->my_env == NULL)
	{
		free_env(mini->env);
		mini->env = NULL;
		return;
	}
	mini->env->my_export = ft_matrix_dup(mini->env->my_export, ev);
	if (mini->env->my_export == NULL)
	{
		free_env(mini->env);
		mini->env = NULL;
		return;
	}
	while (ev[++i])
		if (ft_strnstr(ev[i], "HOME=", 5))
			break;
	mini->env->home = NULL;
	if (ev[i])
		mini->env->home = ft_strdup(ev[i] + 5);
	if (mini->env->home == NULL)
	{
		free_env(mini->env);
		mini->env = NULL;
		return;
	}
	ft_sort_matrix(mini->env->my_export);
	set_shlvl(mini);
}


void my_env_start(t_mini *mini, char **ev)
{
    int k;

    k = 0;
    while (ev[k])
        k++;
    mini->env = ft_calloc(1, sizeof(t_env));
    if (!mini->env)
        return;
    mini->env->my_env = ft_calloc(k + 1, sizeof(char *));
    if (!mini->env->my_env)
    {
        free_env(mini->env);
        mini->env = NULL;
        return;
    }
    mini->env->my_export = ft_calloc(k + 1, sizeof(char *));
    if (!mini->env->my_export)
    {
        free_env(mini->env);
        mini->env = NULL;
        return;
    }
    my_env_continue(mini, ev);
}


t_mini	*mem_save(t_mini *to_save)
{
	static t_mini	*save;

	if (to_save)
		save = to_save;
	return (save);
}
