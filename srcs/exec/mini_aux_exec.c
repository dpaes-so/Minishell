/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_aux_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:59:45 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/06/04 12:12:19 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

int	check_is_dir(char *exec, t_mini *mini, int f)
{
	DIR	*check;

	if (!exec)
		return (0);
	check = opendir(exec);
	if (check && ft_strchr(exec, '/'))
	{
		ft_dprintf(2, "Minishell: %s: Is a directory\n", exec);
		closedir(check);
		if (f == 1)
		{
			if (exec)
				free(exec);
			exit_childprocess(mini, 126);
		}
		return (1);
	}
	if (check)
	{
		freetrix(mini->pipex.path);
		mini->pipex.path = NULL;
		closedir(check);
	}
	return (0);
}

void	wait_child(t_mini *mini)
{
	int	i;
	int	status;
	int	sig;

	status = 0;
	i = 0;
	while (i < mini->cmd_amount && mini->wait_check == 1)
	{
		wait(&status);
		if (WIFEXITED(status))
			mini->pipex.status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			if (sig == SIGINT)
				ft_printf("\n");
			if (sig != SIGPIPE)
				mini->pipex.status = 128 + sig;
		}
		if (mini->pipex.status == 100)
			fmalloc(mini, "wait_child", 2);
		if (mini->execution_signal)
			mini->pipex.status = 130;
		i++;
	}
}

void	cmd_exit_aux(char *exec, t_mini *mini)
{
	if (access(exec, F_OK) < 0)
	{
		ft_dprintf(2, "Minishell: %s: No such file or directory\n", exec);
		exit_childprocess_exec(mini);
		if (exec)
			free(exec);
		exit(127);
	}
	if (access(exec, X_OK) < 0)
	{
		ft_dprintf(2, "Minishell: %s: Permission denied\n", exec);
		exit_childprocess_exec(mini);
		if (exec)
			free(exec);
		exit(126);
	}
}

void	cmd_exit(char *exec, t_mini *mini, char *cmd)
{
	if (!cmd || !*cmd)
	{
		ft_dprintf(2, "Minishell: '%s': command not found\n", cmd);
		exit_childprocess_exec(mini);
		if (exec)
			free(exec);
		exit(127);
	}
	if (ft_strchr(cmd, '/'))
		cmd_exit_aux(exec, mini);
	else
	{
		ft_dprintf(2, "Minishell: %s: command not found\n", cmd);
		exit_childprocess_exec(mini);
		if (exec)
			free(exec);
		exit(127);
	}
}

t_mini	*mem_save(t_mini *to_save)
{
	static t_mini	*save;

	if (to_save)
		save = to_save;
	return (save);
}
