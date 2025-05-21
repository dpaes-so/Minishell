/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export_more_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:47:08 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/21 16:04:08 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

int	export_redirs(t_mini *mini, t_cmd cmds)
{
	int	pid;

	if (mini->cmd_amount == 1)
		mini->wait_check = 0;
	if (do_redirect(&cmds, mini) < 0)
		return (mini->pipex.status = 1, 1);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		if (cmds.fdout != -1)
			dup2(cmds.fdout, STDOUT_FILENO);
		if (!cmds.args[1])
			print_env_ex(mini);
		else
			prep_export(mini, cmds);
		exit_childprocess(mini, 0);
	}
	return (1);
}

char	*get_name(char *arg)
{
	int	len;

	len = 0;
	while (arg[len] && arg[len] != '=')
		len++;
	return (ft_substr(arg, 0, len));
}

int	find_equal(char *in_arr, char *new_arg)
{
	char	*equal_in_arr;
	char	*equal_in_new;

	equal_in_arr = ft_strchr(in_arr, '=');
	equal_in_new = ft_strchr(new_arg, '=');
	if (equal_in_arr != NULL && equal_in_new == NULL)
		return (1);
	return (0);
}

void	*double_check(t_mini *mini, char *arg)
{
	char	**new_env;
	int		size;

	size = 0;
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
	return (NULL);
}

void	*check_exist(int break_point, char *arg, char *key, t_mini *mini)
{
	free(mini->env->my_export[break_point]);
	mini->env->my_export[break_point] = ft_strdup(arg);
	break_point = -1;
	while (mini->env->my_env[++break_point])
	{
		if (!ft_strncmp(mini->env->my_env[break_point], key, ft_strlen(key))
			&& (mini->env->my_env[break_point][ft_strlen(key)] == '='
			|| mini->env->my_env[break_point][ft_strlen(key)] == '\0'))
		{
			free(mini->env->my_env[break_point]);
			mini->env->my_env[break_point] = ft_strdup(arg);
			free(key);
			return (NULL);
		}
	}
	if (mini->env->my_env[break_point] == NULL)
		double_check(mini, arg);
	free(key);
	return (NULL);
}
