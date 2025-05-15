/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_humans.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:27:43 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/14 19:36:40 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

void	here_doc_expand(char *s, t_mini *mini, int fd[2])
{
	int		i;
	int		j;
	char	*s2;

	i = 0;
	while (s[i])
	{
		j = 0;
		if (s[i] == '$')
		{
			i++;
			if (ft_isdigit(s[i]))
			{
				i++;
				continue;
			}
			s2 = find_in_env(s + i, mini);
			while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
				i++;
			while (s2 && s2[j])
				write(fd[1], &s2[j++], 1);
		}
		else
			write(fd[1], &s[i++], 1);
	}
}
void	here_loop(int j, t_cmd *cmds, int fd[2], t_mini *mini)
{
	char	*str;

	while (1)
	{
		str = readline("> ");
		if (!str || !ft_strcmp(str, cmds->redir[j].value))
		{
			free(str);
			break ;
		}
		here_doc_expand(str, mini, fd);
		write(fd[1], "\n", 1);
		free(str);
	}
}
int	here_doc(t_pipe pipex, t_cmd *cmds, int j, t_mini *mini)
{
	int	fd[2];
	int	pid;

	(void)pipex;
	pipe(fd);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		choose_signal(3);
		here_loop(j, cmds, fd, mini);
		exit_childprocess(mini, 0);
	}
	else
		wait(NULL);
	close(fd[1]);
	return (fd[0]);
}

void	first_child(t_mini *mini, t_cmd cmds)
{
	printf("first child\n");
	do_redirect(&cmds, mini);
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
	printf("ultimate child\n");
	do_redirect(&cmds, mini);
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

void	middle_child(t_mini *mini, t_cmd cmds)
{
	printf("in the middle  child\n");
	do_redirect(&cmds, mini);
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
	printf("solo child\n");
	pid = fork();
	if (pid == 0)
	{
		choose_signal(2);
		do_redirect(&cmds, mini);
		if (!cmds.cmd)
			exit_childprocess(mini, 0);
		if (cmds.fdout != -1)
			dup2(cmds.fdout, STDOUT_FILENO);
		if (cmds.fdin != -1)
			dup2(cmds.fdin, STDIN_FILENO);
		cmdexec(mini->env->my_env, cmds, mini);
	}
}
