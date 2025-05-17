/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_aux_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:59:45 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/16 19:24:35 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"


void check_is_dir(char *exec, t_mini *mini)
{
	DIR *check;
	check = opendir(exec);
	if(check)
	{
		ft_printf("Minishell: %s: Is a directory\n",exec);
		closedir(check);
		if(exec)
			free(exec);
		exit_childprocess(mini,126);
	}
}
void	wait_child(t_mini *mini)
{
	int	i;
	int	status;
	int	sig;

	status = 0;
	i = 0;
	while (i < mini->cmd_amount && mini->wait_check == 1)
	{
		wait(&status);
		if (WIFEXITED(status))
			mini->pipex.status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			if (sig == SIGQUIT)
				ft_printf("Quit (core dumped)\n");
			else if (sig == SIGINT)
				ft_printf("\n");
			mini->pipex.status = 128 + sig;
		}
		i++;
	}
}

void	cmd_exit_aux(char *exec, t_mini *mini)
{
	if (access(exec, F_OK) < 0)
	{
		ft_putstr_fd("Pipex: No such file or directory\n", 2);
		exit_childprocess_exec(mini);
		if (exec)
			free(exec);
		exit(127);
	}
	if (access(exec, X_OK) < 0)
	{
		ft_putstr_fd("Pipex: Permission denied\n", 2);
		exit_childprocess_exec(mini);
		if (exec)
			free(exec);
		exit(126);
	}
}

void	cmd_exit(char *exec, t_mini *mini, char *cmd)
{
	if (!cmd || !*cmd)
	{
		ft_putstr_fd("Pipex: command not found\n", 2);
		exit_childprocess_exec(mini);
		if (exec)
			free(exec);
		exit(127);
	}
	if (ft_strchr(cmd, '/'))
		cmd_exit_aux(exec,mini);
	else
	{
		ft_putstr_fd("Pipex: command not found\n", 2);
		exit_childprocess_exec(mini);
		if (exec)
			free(exec);
		exit(127);
	}
}