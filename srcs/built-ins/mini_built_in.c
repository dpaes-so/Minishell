/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:42:40 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/04/29 20:00:40 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

int	do_redirect(t_cmd cmds, int *type)
{
	int	i;
	int	fd;

	fd = 1;
	i = 0;
	while (cmds.redirections[i].value != NULL)
	{
		if (cmds.redirections[i].type == T_OUT_REDIR)
		{
			fd = open(cmds.redirections[i].value, O_CREAT | O_WRONLY | O_TRUNC,
					0644);
			*type = 1;
		}
		else if (cmds.redirections[i].type == T_IN_REDIR)
		{
			fd = open(cmds.redirections[i].value, O_RDONLY, 0644);
			*type = 0;
			if (fd < 0)
			{
				ft_printf("Minishell: %s: No such file or directory\n",cmds.redirections[i].value);
				return (fd);
			}
		}
		else if (cmds.redirections[i].type == T_APPEND_REDIR)
		{
			fd = open(cmds.redirections[i].value, O_CREAT | O_WRONLY | O_APPEND,
					0644);
			*type = 2;
		}
		if (fd < 0)
			ft_putstr_fd("Minishell: Redirect error\n", 2);
		i++;
	}
	return (fd);
}

int	build_pwd(t_mini *mini, t_cmd cmds)
{
	int	fd;
	int	pid;
	int t;

	fd = do_redirect(cmds,&t);
	get_pwd(mini);
	if(fd == 1)
		ft_printf("%s\n", mini->pwd);
	else
	{
		pid = fork();
		if (pid == 0)
		{
			if (t == 0)
				return(1);
			else
				dup2(fd, STDOUT_FILENO);
			ft_printf("%s\n", mini->pwd);
			exit_childprocess(mini);
		}
		else
			wait(NULL);
	}
	return (1);
}
