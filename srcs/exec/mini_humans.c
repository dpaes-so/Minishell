/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_humans.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:27:43 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/13 13:59:06 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

int	here_doc(t_pipe pipex, t_cmd *cmds,int j)
{
	char	*str;
	int		fd[2];
	int		i;

	(void)pipex;
	pipe(fd);
	choose_signal(3);
	while (1)
	{
		i = 0;
		str = readline("> ");
		if (!str || !ft_strncmp(str, cmds->redir[j].value,
				ft_strlen(cmds->redir[j].value)))
		{
			free(str);
			break ;
		}
		while (str[i])
			write(fd[1], &str[i++], 1);
		write(fd[1], "\n", 1);
		free(str);
	}
	close(fd[1]);
	return (fd[0]);
}

// int	here_doc(t_pipe pipex, t_cmd *cmds)
// {
// 	char	*str;
// 	int		fd[2];
// 	int		i;

// 	(void)pipex;
// 	pipe(fd);
// 	choose_signal(3);
// 	while (1)
// 	{
// 		i = 0;
// 		str = readline("> ");
// 		if (!str || !ft_strncmp(str, cmds->redir[0].value,
// 				ft_strlen(cmds->redir[0].value)))
// 		{
// 			free(str);
// 			break ;
// 		}
// 		while (str[i])
// 			write(fd[1], &str[i++], 1);
// 		write(fd[1], "\n", 1);
// 		free(str);
// 	}
// 	close(fd[1]);
// 	return (fd[0]);
// }

void	first_child(t_mini *mini, t_cmd cmds)
{
	do_redirect(&cmds, mini);
	if (!cmds.cmd)
		exit_childprocess(mini,0);
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
	do_redirect(&cmds, mini);
	if (!cmds.cmd)
		exit_childprocess(mini,0);
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

void	middle_child(t_mini *mini, t_cmd cmds)
{
	do_redirect(&cmds, mini);
	if (!cmds.cmd)
		exit_childprocess(mini,0);
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
	else
	{
		dup2(mini->save_fd, STDIN_FILENO);
		close(mini->pipex.pipefd[0]);
	}
	cmdexec(mini->env->my_env, cmds, mini);
}

void	solo_child(t_mini *mini, t_cmd cmds)
{
	int	pid;

	signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		mem_save(mini);
		choose_signal(2);
		do_redirect(&cmds, mini);
		if (!cmds.cmd)
			exit_childprocess(mini,0);
		if (cmds.fdout != -1)
			dup2(cmds.fdout, STDOUT_FILENO);
		if (cmds.fdin != -1)
			dup2(cmds.fdin, STDIN_FILENO);
		cmdexec(mini->env->my_env, cmds, mini);
	}
}
