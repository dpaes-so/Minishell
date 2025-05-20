/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:32:35 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/20 19:58:23 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"


static void	*finish_export(t_mini *mini, char *arg)
{
	char	**new_env;
	char	**new_export;
	int		size;
	int		b_point;

	b_point = -1;
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
static void	*make_export(t_mini *mini, char *arg, int f)
{
	int		break_point;

	break_point = -1;
	while (mini->env->my_export[++break_point])
		if (!ft_strncmp(mini->env->my_export[break_point], arg, ft_strchr(arg, '=')- arg) || f == 2)
		{
			if (f == 2)
				return (add_export(mini, arg));
			else
			{
				free(mini->env->my_export[break_point]);
				mini->env->my_export[break_point] = ft_strdup(arg);
				break_point = -1;
				while(mini->env->my_env[++break_point])
					if (!ft_strncmp(mini->env->my_env[break_point], arg, ft_strchr(arg, '=')- arg))
					{
						free(mini->env->my_env[break_point]);
						mini->env->my_env[break_point] = ft_strdup(arg);
						break;
					}
				if(mini->env->my_env[break_point] == NULL)
					double_check(mini,arg);
				return (NULL);
			}
		}
	return (finish_export(mini,arg),NULL);
}

static void	prep_export(t_mini *mini, t_cmd cmds)
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
			ft_printf("Minishell: '%s' not a valid identifier\n", arg);
			mini->pipex.status = 1;
			j++;
			continue ;
		}
		make_export(mini, arg, i);
		j++;
	}
}

static int	export_redirs(t_mini *mini, t_cmd cmds)
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
