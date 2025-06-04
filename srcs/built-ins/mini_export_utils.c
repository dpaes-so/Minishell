/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 16:51:17 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/06/04 13:59:33 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

static void	*add_exp_fail_fr(char *arg, char **env, int i)
{
	char	*new_arg;

	new_arg = remove_plus_sign(arg);
	env[i] = ft_strdup(new_arg);
	free(new_arg);
	if (!env[i])
	{
		while (--i >= 0)
			free(env[i]);
		free(env);
		return (NULL);
	}
	env[++i] = NULL;
	return (NULL);
}

static char	**add_exp_fail(char **env, char **env2, int size, char *arg)
{
	int	i;

	i = -1;
	env = ft_calloc((size + 2), sizeof(char *));
	if (!env)
		return (NULL);
	while (env2[++i])
	{
		env[i] = ft_strdup(env2[i]);
		if (!env[i])
		{
			while (--i >= 0)
				free(env[i]);
			free(env);
			return (NULL);
		}
	}
	add_exp_fail_fr(arg, env, i);
	return (env);
}

void	*add_exp_fr(t_mini *mini, int b_point, char *arg)
{
	int	equal_check;

	equal_check = 0;
	if (ft_strchr(mini->env->my_export[b_point], '='))
		equal_check = 1;
	mini->env->my_export[b_point] = ft_strjoin(mini->env->my_export[b_point],
			ft_strchr(arg, '+') + equal_check + 1);
	if (!mini->env->my_export[b_point])
		fmalloc(mini, "add_exp_fr", 2);
	return (NULL);
}

void	*add_exp_agn(t_mini *mini, char *arg)
{
	char	**new_export;
	int		size;
	int		b_point;

	size = 0;
	b_point = -1;
	new_export = NULL;
	while (mini->env->my_export[size])
		size++;
	while (mini->env->my_export[++b_point])
	{
		if (!ft_strncmp(mini->env->my_export[b_point], arg, ft_strchr(arg, '+')
				- arg))
		{
			return (add_exp_fr(mini, b_point, arg));
		}
	}
	new_export = add_exp_fail(new_export, mini->env->my_export, size, arg);
	if (!new_export)
		fmalloc(mini, "add_exp_fail", 2);
	freetrix(mini->env->my_export);
	mini->env->my_export = new_export;
	return (NULL);
}

void	*add_export(t_mini *mini, char *arg, int size, int b_point)
{
	char	**new_env;

	new_env = NULL;
	while (mini->env->my_env[size])
		size++;
	while (mini->env->my_env[++b_point])
	{
		if (!ft_strncmp(mini->env->my_env[b_point], arg, ft_strchr(arg, '+')
				- arg))
		{
			mini->env->my_env[b_point] = ft_strjoin(mini->env->my_env[b_point],
					ft_strchr(arg, '+') + 2);
			if (!mini->env->my_env[b_point])
				fmalloc(mini, "add_export", 2);
			return (add_exp_agn(mini, arg), NULL);
		}
	}
	new_env = add_exp_fail(new_env, mini->env->my_env, size, arg);
	if (!new_env)
		fmalloc(mini, "add_export", 2);
	freetrix(mini->env->my_env);
	mini->env->my_env = new_env;
	return (add_exp_agn(mini, arg), NULL);
}
