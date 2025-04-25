/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:42:40 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/04/16 17:40:48 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

int do_redirect(t_cmd cmds)
{
	int i;
	int fd;

	fd = 1;
	i = 0;
	while (cmds.redirections[i].value != NULL)
	{
		fd = open(cmds.redirections[i].value, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if(fd < 0)
			ft_putstr_fd("Minishell: Redirect error\n",2);
		i++;
	}
	return(fd);
}

void	build_echo(t_mini *mini)
{
	if (ft_strncmp(mini->input + 5, "-n ", 3) == 0)
		ft_printf("%s", mini->input + 8);
	else
		ft_printf("%s\n", mini->input + 5);
}

int	build_pwd(t_mini *mini,t_cmd cmds)
{
	int fd;
	int pid;

	fd = do_redirect(cmds);
	get_pwd(mini);
	pid = fork();
	if(pid == 0)
	{	
		if(fd != 1)
			dup2(fd,STDOUT_FILENO);
		ft_printf("%s\n", mini->pwd);
		free(mini->pwd);
		free(mini->env->home);
		freetrix(mini->env->my_env);
		free(mini->env);
		free_tree(mini->ast);
		clear_history();
		master_close();
		exit(0);
	}
	else
		wait(NULL);
	return (1);
}
