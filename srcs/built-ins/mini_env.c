/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:31:00 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/04/14 16:07:05 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

int	build_env(t_mini *mini,t_cmd cmds)
{
	int	i;
	int fd;
	int pid;

	i = -1;
	fd = do_redirect(cmds);
	if(cmds.amount > 0)
		return(1);
	get_pwd(mini);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if(pid == 0)
	{	
		if(fd != 1)
			dup2(fd,STDOUT_FILENO);
		while (mini->env->my_env[++i])
			ft_printf("%s\n", mini->env->my_env[i]);
		exit_childprocess(mini);
	}
	else
		wait(NULL);
	return (1);
}

//export function but no space, leave me alone >:(
int	print_env_ex(t_mini *mini)
{
	int	i;

	i = -1;
	while (mini->env->my_env[++i])
		ft_printf("declare -x %s\n", mini->env->my_env[i]);
	return (1);
}