/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:31:00 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/04/29 20:01:47 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

static int redir_env(int fd,t_mini *mini,int t)
{
	int pid;
	int i;
	
	i = -1;
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if(pid == 0)
	{	
		if (t == 0)
			return(1);
		else
			dup2(fd, STDOUT_FILENO);
		while (mini->env->my_env[++i])
			ft_printf("%s\n", mini->env->my_env[i]);
		exit_childprocess(mini);
	}
	else
		wait(NULL);
	return (1);
}

int normal_env(t_mini *mini)
{
	int i;
	
	i = -1;
	while (mini->env->my_env[++i])
			ft_printf("%s\n", mini->env->my_env[i]);
	return (1);
}
int	build_env(t_mini *mini,t_cmd cmds)
{
	int fd;
	int res;
	int t;
	
	fd = do_redirect(cmds,&t);
	get_pwd(mini);
	pwd_update(mini);
	if(cmds.amount > 1)
		return(1);
	if(fd != 1)
		res = redir_env(fd,mini,t);
	else
		res = normal_env(mini);
	return(res);
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