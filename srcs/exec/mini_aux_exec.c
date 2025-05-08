/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_aux_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:59:45 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/08 15:27:53 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

void	wait_child(t_mini *mini)
{
	int	i;
	int	status;
	int sig;

	status = 0;
	i = 0;
	while (i < mini->cmd_amount)
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


char	**matrix_dup(t_mini *mini, char **ev)
{
	int	j;

	j = -1;
	while (ev[++j])
	{
		mini->env->my_env[j] = ft_strdup(ev[j]);
		if (mini->env->my_env[j] == NULL)
			return (NULL);
	}
	return (mini->env->my_env);
}

void	set_shlvl(t_mini *mini)
{
	int		sh_lvl;
	char	*temp;
	char	*shlvl;
	int		i;

	i = -1;
	while (mini->env->my_env[++i])
		if (ft_strnstr(mini->env->my_env[i], "SHLVL=", 6))
			break ;
	sh_lvl = ft_atoi(mini->env->my_env[i] + 6);
	sh_lvl++;
	shlvl = ft_itoa(sh_lvl);
	temp = ft_strjoin(ft_strdup("SHLVL="), shlvl);
	free(mini->env->my_env[i]);
	mini->env->my_env[i] = ft_strdup(temp);
	free(temp);
	free(shlvl);
}
void	my_env_start(t_mini *mini, char **ev)
{
	int	i;
	int	k;

	i = -1;
	k = 0;
	mini->env = malloc(sizeof(t_env));
	if (mini->env == NULL)
		return ;
	while (ev[k])
		k++;
	mini->env->my_env = (char **)ft_calloc(k + 1, sizeof(char *));
	if (mini->env->my_env == NULL)
		return ;
	mini->env->my_env = matrix_dup(mini, ev);
	if (mini->env->my_env == NULL)
		return ;
	while (ev[++i])
		if (ft_strnstr(ev[i], "HOME=", 5))
			break ;
	mini->env->home = NULL;
	if (ev[i])
		mini->env->home = ft_strdup(ev[i] + 5);
	if (mini->env->home == NULL)
		return ;
	set_shlvl(mini);
}

void	cmd_exit(char *exec, t_mini *mini)
{
	if (access(exec, F_OK) < 0)
	{
		ft_putstr_fd("Pipex: Command not found\n", 2);
		exit_childprocess_exec(mini);
		if (exec)
			free(exec);
		exit(127);
	}
	if (access(exec, X_OK) < 0)
	{
		ft_putstr_fd("Permission  2 denied\n", 2);
		exit_childprocess_exec(mini);
		if (exec)
			free(exec);
		exit(126);
	}
}

t_mini	*mem_save(t_mini *to_save)
{
	static t_mini	*save;

	if (to_save)
		save=to_save;
	return (save);
}
