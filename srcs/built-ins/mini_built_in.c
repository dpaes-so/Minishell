/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:42:40 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/06/04 11:26:56 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

int	check_value(t_cmd *cmds, int i, int *fd)
{
	*fd = 0;
	if (cmds->redir[i].value[0] == '\0')
		return (ft_dprintf(2, "Minishell: %s: No such file or directory\n",
				cmds->redir[i].value), -2);
	return (1);
}

int	redir_check(t_cmd *cmds, int i)
{
	int	fd;

	if (check_value(cmds, i, &fd) < 0)
		return (-2);
	if (cmds->redir[i].type == T_OUT_REDIR)
	{
		fd = open(cmds->redir[i].value, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		cmds->fdout = fd;
	}
	else if (cmds->redir[i].type == T_IN_REDIR)
	{
		fd = open(cmds->redir[i].value, O_RDONLY, 0644);
		cmds->fdin = fd;
	}
	else if (cmds->redir[i].type == T_APPEND_REDIR)
	{
		fd = open(cmds->redir[i].value, O_CREAT | O_WRONLY | O_APPEND, 0644);
		cmds->fdout = fd;
	}
	else if (cmds->redir[i].type == T_HERE_DOC)
		cmds->fdin = cmds->here_fd;
	if (fd < 0)
		return (ft_dprintf(2, "Minishell: "), perror(cmds->redir[i].value), -1);
	return (fd);
}

int	do_redirect(t_cmd *cmds, t_mini *mini)
{
	int	i;
	int	fd;

	fd = 1;
	i = -1;
	while (cmds->redir && cmds->redir[++i].value != NULL)
	{
		if (cmds->redir[i].ambiguous)
		{
			ft_dprintf(2, "Minishell: %s: ambiguous redirect\n",
				cmds->redir[i].value);
			exit_childprocess(mini, 1);
		}
		fd = redir_check(cmds, i);
		if (fd == -1)
			break ;
	}
	return (fd);
}

void	get_pwd(t_mini *mini)
{
	char	*cdw;

	cdw = NULL;
	cdw = getcwd(cdw, 100);
	free(mini->pwd);
	mini->pwd = ft_strjoin(cdw, "");
	if (!mini->pwd)
		fmalloc(mini, "get_pwd", 2);
}

int	build_pwd(t_mini *mini, t_cmd cmds)
{
	int	pid;
	int	fd;

	fd = do_redirect(&cmds, mini);
	if (fd < 0)
		return (mini->wait_check = 0, mini->pipex.status = 1, 1);
	get_pwd(mini);
	if (cmds.fdout == -1)
		ft_printf("%s\n", mini->pwd);
	else
	{
		pid = fork();
		if (pid == 0)
		{
			if (cmds.fdout == -1)
				return (1);
			else
				dup2(cmds.fdout, STDOUT_FILENO);
			ft_printf("%s\n", mini->pwd);
			exit_childprocess(mini, 0);
		}
	}
	return (1);
}
