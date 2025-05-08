/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:31:00 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/05 13:04:00 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

static int	redir_env(int fd, t_mini *mini, int t)
{
	int	pid;
	int	i;

	i = -1;
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		if (t == -1)
			return (1);
		else
			dup2(fd, STDOUT_FILENO);
		while (mini->env->my_env[++i])
			ft_printf("%s\n", mini->env->my_env[i]);
		exit_childprocess(mini,0);
	}
	else
		wait(NULL);
	return (1);
}

int	normal_env(t_mini *mini)
{
	int	i;

	i = -1;
	while (mini->env->my_env[++i])
		ft_printf("%s\n", mini->env->my_env[i]);
	return (1);
}

int	build_env(t_mini *mini, t_cmd cmds)
{
	int	res;

	do_redirect(&cmds, mini);
	get_pwd(mini);
	pwd_update(mini);
	if (cmds.amount > 1)
		return (ft_putstr_fd("too many arguments", 2), 1);
	if (cmds.fdout != -1)
		res = redir_env(cmds.fdout, mini, cmds.fdout);
	else
		res = normal_env(mini);
	return (res);
}

// export function but no space, leave me alone >:(
int	print_env_ex(t_mini *mini)
{
	int	i;

	i = -1;
	while (mini->env->my_env[++i])
		ft_printf("declare -x %s\n", mini->env->my_env[i]);
	return (1);
}
