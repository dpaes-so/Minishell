/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_more_aux_exec.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 14:30:30 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/06/02 17:32:23 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

void	exit_childprocess(t_mini *mini, int ecode)
{
	if (mini->pwd)
		free(mini->pwd);
	if (mini->env)
	{
		if (mini->env->my_env)
			freetrix(mini->env->my_env);
		if (mini->env->my_export)
			freetrix(mini->env->my_export);
		if (mini->env)
			free(mini->env);
	}
	if (ecode != -2)
	{
		if (mini->ast)
			free_tree(mini->ast);
	}
	else
		ecode = 0;
	if (mini->pipex.path)
		freetrix(mini->pipex.path);
	clear_history();
	master_close();
	exit(ecode);
}

void	exit_childprocess_exec(t_mini *mini)
{
	if (mini->pwd)
		free(mini->pwd);
	if (mini->env->my_env)
		freetrix(mini->env->my_env);
	if (mini->env->my_export)
		freetrix(mini->env->my_export);
	if (mini->env)
		free(mini->env);
	if (mini->ast)
		free_tree(mini->ast);
	if (mini->pipex.path)
		free(mini->pipex.path);
	clear_history();
	master_close();
}

char	*get_path(t_cmd cmds, t_mini *mini, int i, int *flag)
{
	char	*exec;

	if (mini->pipex.path != NULL && mini->pipex.path[i] && (access(cmds.cmd,
				F_OK | X_OK) < 0))
	{
		exec = ft_strjoin(mini->pipex.path[i], cmds.cmd);
	}
	else
	{
		exec = ft_strdup(cmds.cmd);
		check_is_dir(exec, mini, 1);
		(*flag) = 1;
	}
	return (exec);
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
