/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:27:03 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/06/02 17:02:18 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

static void	redir_echo(t_cmd cmds, int flag, int fd)
{
	int	i;

	i = 1;
	if (fd < 0)
		return ;
	if (cmds.fdout != -1)
		dup2(cmds.fdout, STDOUT_FILENO);
	if (flag != 0)
		i = flag;
	if (cmds.amount == 0)
		ft_printf("\n");
	else
	{
		while (cmds.args[i])
		{
			ft_printf("%s", cmds.args[i]);
			i++;
			if (!cmds.args[i])
				break ;
			ft_printf(" ");
		}
		if (flag == 0)
			ft_printf("\n");
	}
}

static void	normal_echo(t_mini *mini, t_cmd cmds, int flag)
{
	int	i;

	i = 1;
	(void)mini;
	if (flag != 0)
		i = flag;
	if (cmds.amount == 0)
		ft_printf("\n");
	else
	{
		while (cmds.args[i])
		{
			ft_printf("%s", cmds.args[i]);
			i++;
			if (!cmds.args[i])
				break ;
			ft_printf(" ");
		}
		if (flag == 0)
			ft_printf("\n");
	}
}

static int	echo_flag(t_cmd cmds)
{
	int	i;
	int	j;

	i = 1;
	j = 1;
	if (cmds.args[j][i] == '\0')
		return (0);
	while (cmds.args[j][i] == 'n')
		i++;
	if (cmds.args[j][i] != '\0')
		return (0);
	j = 2;
	while (cmds.args[j] && cmds.args[j][0] == '-')
	{
		i = 1;
		if (cmds.args[j][i] == '\0')
			return (j);
		while (cmds.args[j][i] == 'n')
			i++;
		if (cmds.args[j][i] != '\0')
			return (j);
		j++;
	}
	return (j);
}

int	build_echo(t_mini *mini, t_cmd cmds)
{
	int	flag;
	int	pid;
	int	fd;

	flag = 0;
	if (cmds.amount != 1)
		if (cmds.args && cmds.args[1][0] == '-')
			flag = echo_flag(cmds);
	if (cmds.redir[0].type != T_NULL)
	{
		fd = do_redirect(&cmds, mini);
		pid = fork();
		if (pid == 0)
		{
			redir_echo(cmds, flag, fd);
			if (fd >= 0)
				fd = 0;
			else
				fd = 1;
			exit_childprocess(mini, fd);
		}
	}
	else
		normal_echo(mini, cmds, flag);
	return (1);
}
