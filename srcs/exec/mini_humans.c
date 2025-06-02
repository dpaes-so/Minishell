/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_humans.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:27:43 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/06/02 17:48:52 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

void	first_child(t_mini *mini, t_cmd cmds)
{
	int	fd;

	fd = do_redirect(&cmds, mini);
	if (fd < 0)
		exit_childprocess(mini, 1);
	if (!cmds.cmd || fd < 0)
		exit_childprocess(mini, 0);
	if (cmds.fdout != -1)
	{
		dup2(cmds.fdout, STDOUT_FILENO);
		close(cmds.fdout);
	}
	else
	{
		dup2(mini->pipex.pipefd[1], STDOUT_FILENO);
		close(mini->pipex.pipefd[1]);
	}
	if (cmds.fdin != -1)
	{
		dup2(cmds.fdin, STDIN_FILENO);
		close(cmds.fdin);
	}
	close(mini->pipex.pipefd[0]);
	cmdexec(mini->env->my_env, cmds, mini);
}

void	last_child(t_mini *mini, t_cmd cmds)
{
	int	fd;

	fd = do_redirect(&cmds, mini);
	if (fd < 0)
		exit_childprocess(mini, 1);
	if (!cmds.cmd)
		exit_childprocess(mini, 0);
	if (cmds.fdout != -1)
	{
		dup2(cmds.fdout, STDOUT_FILENO);
		close(cmds.fdout);
	}
	if (cmds.fdin != -1)
	{
		dup2(cmds.fdin, STDIN_FILENO);
		close(cmds.fdin);
	}
	else
	{
		dup2(mini->save_fd, STDIN_FILENO);
		close(mini->pipex.pipefd[0]);
	}
	close(mini->pipex.pipefd[1]);
	cmdexec(mini->env->my_env, cmds, mini);
}

static void	fdin_middlec_check(t_cmd cmds, t_mini *mini)
{
	if (cmds.fdin != -1)
	{
		dup2(cmds.fdin, STDIN_FILENO);
		close(cmds.fdin);
	}
	else
	{
		dup2(mini->save_fd, STDIN_FILENO);
		close(mini->pipex.pipefd[0]);
	}
}

void	middle_child(t_mini *mini, t_cmd cmds)
{
	int	fd;

	fd = do_redirect(&cmds, mini);
	if (fd < 0)
		exit_childprocess(mini, 1);
	if (!cmds.cmd)
		exit_childprocess(mini, 0);
	if (cmds.fdout != -1)
	{
		dup2(cmds.fdout, STDOUT_FILENO);
		close(cmds.fdout);
	}
	else
	{
		dup2(mini->pipex.pipefd[1], STDOUT_FILENO);
		close(mini->pipex.pipefd[1]);
	}
	fdin_middlec_check(cmds, mini);
	master_close();
	cmdexec(mini->env->my_env, cmds, mini);
}

void	solo_child(t_mini *mini, t_cmd cmds)
{
	int	pid;
	int	fd;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		signals(2);
		fd = do_redirect(&cmds, mini);
		if (fd < 0)
			exit_childprocess(mini, 1);
		if (!cmds.cmd)
			exit_childprocess(mini, 0);
		if (cmds.fdout != -1)
			dup2(cmds.fdout, STDOUT_FILENO);
		if (cmds.fdin != -1)
			dup2(cmds.fdin, STDIN_FILENO);
		cmdexec(mini->env->my_env, cmds, mini);
	}
}
