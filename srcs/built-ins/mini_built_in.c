/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:42:40 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/05/27 12:28:17 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

int	check_value(t_cmd *cmds, int i)
{
	if (cmds->redir[i].value[0] == '\0')
		return (ft_dprintf(2, "Minishell: %s: No such file or directory\n",
				cmds->redir[i].value), -2);
}

int	redir_check(t_cmd *cmds, int i)
{
	int	fd;

	fd = 0;
	check_value(cmds, i);
	if (cmds->redir[i].type == T_OUT_REDIR)
	{
		fd = open(cmds->redir[i].value, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		cmds->fdout = fd;
	}
	else if (cmds->redir[i].type == T_IN_REDIR)
	{
		fd = open(cmds->redir[i].value, O_RDONLY, 0644);
		cmds->fdin = fd;
		if (fd < 0)
			return (ft_dprintf(2, "Minishell: %s: No such file or directory\n",
					cmds->redir[i].value), -2);
	}
	else if (cmds->redir[i].type == T_APPEND_REDIR)
	{
		fd = open(cmds->redir[i].value, O_CREAT | O_WRONLY | O_APPEND, 0644);
		cmds->fdout = fd;
	}
	else if (cmds->redir[i].type == T_HERE_DOC)
		cmds->fdin = cmds->here_fd;
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
		fd = redir_check(cmds, i);
		if (fd == -2)
		{
			fd = -1;
			break ;
		}
		else if (fd < 0)
		{
			if (!check_is_dir(cmds->redir[i].value, mini, 0))
			{
				ft_dprintf(2, "Minishell: %s: Permission denied\n",
					cmds->redir[i].value);
			}
		}
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
