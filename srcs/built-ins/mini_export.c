/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:32:35 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/21 15:02:50 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

void	*finish_fr(int size, char **new_export, char *arg, t_mini *mini)
{
	size = 0;
	while (mini->env->my_export[size])
		size++;
	new_export = ft_calloc((size + 2), sizeof(char *));
	if (!new_export)
		return (NULL);
	new_export = ft_matrix_dup(new_export, mini->env->my_export);
	new_export[size++] = ft_strdup(arg);
	new_export[size] = NULL;
	freetrix(mini->env->my_export);
	mini->env->my_export = new_export;
	return (NULL);
}

static void	*finish_export(t_mini *mini, char *arg)
{
	char	**new_env;
	char	**new_export;
	int		size;

	size = 0;
	new_export = NULL;
	if (ft_strchr(arg, '='))
	{
		while (mini->env->my_env[size])
			size++;
		new_env = ft_calloc((size + 2), sizeof(char *));
		if (!new_env)
			return (NULL);
		new_env = ft_matrix_dup(new_env, mini->env->my_env);
		new_env[size++] = ft_strdup(arg);
		new_env[size] = NULL;
		freetrix(mini->env->my_env);
		mini->env->my_env = new_env;
	}
	finish_fr(size, new_export, arg, mini);
	return (NULL);
}

static void	*make_export(t_mini *mini, char *arg, int f)
{
	int		break_point;
	char	*key;

	key = get_name(arg);
	if (!key)
		return (NULL);
	break_point = -1;
	while (mini->env->my_export[++break_point])
	{
		if ((!ft_strncmp(mini->env->my_export[break_point], key, ft_strlen(key))
				&& (mini->env->my_export[break_point][ft_strlen(key)] == '='
				|| mini->env->my_export[break_point][ft_strlen(key)] == '\0'))
			|| f == 2)
		{
			if (find_equal(mini->env->my_export[break_point], arg))
				return (free(key), NULL);
			if (f == 2)
				return (free(key), add_export(mini, arg));
			return (check_exist(break_point, arg, key, mini));
		}
	}
	free(key);
	return (finish_export(mini, arg), NULL);
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
