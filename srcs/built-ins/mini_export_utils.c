/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniel <daniel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 16:51:17 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/06/01 01:35:00 by daniel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

int	check_char(char *s, int *ctd, int i)
{
	if (!((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')
			|| (s[i] >= '0' && s[i] <= '9') || (s[i] == '_') || s[i] == '+'
			|| s[i] == '=') || ((*ctd) == 1 && s[i] != '='))
		return (0);
	if (s[i] == '+')
		(*ctd)++;
	if ((*ctd) == 2)
		return (0);
	return (1);
}

static char	**add_exp_fail(char **env, char **env2, int size, char *arg)
{
	char	*new_arg;
	int		i;

	i = -1;
	env = ft_calloc((size + 2), sizeof(char *));
	if (!env)
		return(NULL);
	while (env2[++i])
	{
		env[i] = ft_strdup(env2[i]);
		if(!env[i])
		{
			while(--i >= 0)
				free(env[i]);
			free(env);
			return(NULL);
		}
	}
	new_arg = remove_plus_sign(arg);
	env[i] = ft_strdup(new_arg);
	free(new_arg);
	if(!env[i])
	{
		while(--i >= 0)
			free(env[i]);
		free(env);
		return(NULL);
	}
	env[++i] = NULL;
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
	if(!mini->env->my_export[b_point])
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
	return (mini->env->my_export = new_export, NULL);
}

void	*add_export(t_mini *mini, char *arg)
{
	char	**new_env;
	int		size;
	int		b_point;

	size = 0;
	b_point = -1;
	new_env = NULL;
	while (mini->env->my_env[size])
		size++;
	while (mini->env->my_env[++b_point])
	{
		if (!ft_strncmp(mini->env->my_env[b_point], arg, ft_strchr(arg, '+')- arg))
		{
			mini->env->my_env[b_point] = ft_strjoin(mini->env->my_env[b_point],ft_strchr(arg, '+') + 2);
			if(!mini->env->my_env[b_point])
				fmalloc(mini, "add_export", 2);
			return (add_exp_agn(mini, arg), NULL);
		}
	}
	new_env = add_exp_fail(new_env, mini->env->my_env, size, arg);
	if (!new_env)
		fmalloc(mini, "add_export", 2);
	freetrix(mini->env->my_env);
	return (add_exp_agn(mini, arg), mini->env->my_env = new_env, NULL);
}
