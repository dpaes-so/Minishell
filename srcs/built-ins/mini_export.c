/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:32:35 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/05 13:04:11 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

static int	check_valid_variable_name(char *s)
{
	int	i;

	i = 0;
	if (s[0] == '=')
		return (0);
	if (!((s[0] >= 'a' && s[0] <= 'z') || (s[0] >= 'A' && s[0] <= 'Z')
			|| (s[0] == '_')))
		return (0);
	while (s[i] && s[i] != '=')
	{
		if (!((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')
				|| (s[i] >= '0' && s[i] <= '9') || (s[i] == '_')))
			return (0);
		i++;
	}
	return (i);
}

static void	*make_export(t_mini *mini, char *arg)
{
	char	**new_env;
	int		size;
	int		break_point;
	int		i;

	size = 0;
	i = -1;
	break_point = -1;
	while (mini->env->my_env[size])
		size++;
	while (mini->env->my_env[++break_point])
		if (!ft_strncmp(mini->env->my_env[break_point], arg, ft_strchr(arg, '=')
				- arg))
			return (free(mini->env->my_env[break_point]),
				mini->env->my_env[break_point] = ft_strdup(arg), NULL);
	new_env = malloc(sizeof(char *) * (size + 2));
	if (!new_env)
		return (NULL);
	while (mini->env->my_env[++i])
		new_env[i] = ft_strdup(mini->env->my_env[i]);
	new_env[i++] = ft_strdup(arg);
	new_env[i] = NULL;
	freetrix(mini->env->my_env);
	mini->env->my_env = new_env;
	return (NULL);
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
			j++;
			continue ;
		}
		make_export(mini, arg);
		j++;
	}
}

static int	export_redirs(t_mini *mini, t_cmd cmds)
{
	int	pid;

	do_redirect(&cmds, mini);
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
		exit_childprocess(mini,0);
	}
	else
		wait(NULL);
	return (1);
}

int	build_export(t_mini *mini, t_cmd cmds)
{
	if (cmds.redir[0].type != T_NULL)
		export_redirs(mini, cmds);
	else
	{
		if (!cmds.args[1])
			print_env_ex(mini);
		prep_export(mini, cmds);
	}
	return (1);
}
