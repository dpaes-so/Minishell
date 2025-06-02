/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:32:35 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/06/02 16:25:08 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

void	*finish_fr(int size, char *arg, t_mini *mini)
{
	char	**new_export;

	size = 0;
	while (mini->env->my_export[size])
		size++;
	new_export = ft_calloc((size + 2), sizeof(char *));
	if (!new_export)
	{
		free(new_export);
		fmalloc(mini, "finish_fr", 2);
	}
	new_export = ft_matrix_dup(new_export, mini->env->my_export);
	if (!new_export)
	{
		freetrix(new_export);
		fmalloc(mini, "finish_fr", 2);
	}
	finish_on_god(new_export, arg, mini, size);
	return (NULL);
}

static void	*finish_export(t_mini *mini, char *arg, int size)
{
	char	**new_env;

	if (ft_strchr(arg, '='))
	{
		while (mini->env->my_env[size])
			size++;
		new_env = ft_calloc((size + 2), sizeof(char *));
		if (!new_env)
			fmalloc(mini, "finish_export", 2);
		if (!ft_matrix_dup(new_env, mini->env->my_env))
			fmalloc(mini, "finish_export", 2);
		new_env[size] = ft_strdup(arg);
		if (!new_env[size])
		{
			while (--size >= 0)
				free(new_env[size]);
			free(new_env);
			fmalloc(mini, "finish_export", 2);
		}
		new_env[size + 1] = NULL;
		freetrix(mini->env->my_env);
		mini->env->my_env = new_env;
	}
	return (finish_fr(size, arg, mini), NULL);
}

static void	*make_export(t_mini *mini, char *arg, int f)
{
	int		b_point;
	char	*key;

	key = get_name(arg, mini);
	if (!key)
		return (NULL);
	b_point = -1;
	while (mini->env->my_export[++b_point])
	{
		if ((!ft_strncmp(mini->env->my_export[b_point], key, ft_strlen(key))
				&& (mini->env->my_export[b_point][ft_strlen(key)] == '='
				|| mini->env->my_export[b_point][ft_strlen(key)] == '\0'))
			|| f == 2)
		{
			if (find_equal(mini->env->my_export[b_point], arg))
				return (free(key), NULL);
			if (f == 2)
				return (free(key), add_export(mini, arg, 0, -1));
			return (check_exist(b_point, arg, key, mini));
		}
	}
	free(key);
	return (finish_export(mini, arg, 0), NULL);
}

void	prep_export(t_mini *mini, t_cmd cmds)
{
	int		j;
	int		i;
	char	*arg;

	j = 1;
	while (cmds.args[j])
	{
		arg = cmds.args[j];
		i = check_valid_variable_name(arg);
		if (!i)
		{
			ft_dprintf(2, "Minishell: '%s' not a valid identifier\n", arg);
			mini->pipex.status = 1;
			j++;
			continue ;
		}
		make_export(mini, arg, i);
		j++;
	}
}

int	build_export(t_mini *mini, t_cmd cmds)
{
	mini->pipex.status = 0;
	if (mini->cmd_amount == 1)
		mini->wait_check = 0;
	if (cmds.redir[0].type != T_NULL)
		export_redirs(mini, cmds);
	else
	{
		if (!cmds.args[1])
			print_env_ex(mini);
		else
			prep_export(mini, cmds);
	}
	return (1);
}
