/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_aux.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:58:23 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/08 18:33:55 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/mini_header.h"

void	freetrix(char **matrix)
{
	int	i;

	i = 0;
	if (!matrix)
		return ;
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
	if (matrix)
		free(matrix);
}

void	master_close(void)
{
	int	i;

	i = 2;
	while (++i < 1024)
		close(i);
}

void	exit_childprocess(t_mini *mini,int ecode)
{
	free(mini->pwd);
	if (mini->env->home != NULL)
		free(mini->env->home);
	freetrix(mini->env->my_env);
	free(mini->env);
	free_tree(mini->ast);
	freetrix(mini->pipex.path);
	clear_history();
	master_close();
	exit(ecode);
}

void	exit_childprocess_exec(t_mini *mini)
{
	free(mini->pwd);
	if (mini->env->home != NULL)
		free(mini->env->home);
	freetrix(mini->env->my_env);
	free(mini->env);
	free_tree(mini->ast);
	free(mini->pipex.path);
	clear_history();
	master_close();
}

char	**path_finder(char **envp)
{
	int		i;
	char	*temp;
	char	**split;

	i = -1;
	while (envp[++i])
		if (ft_strnstr(envp[i], "PATH", 4))
			break ;
	if (!envp[i])
	{
		return(NULL);
	}
	envp[i] = envp[i] + 5;
	split = ft_split(envp[i], ':');
	i = -1;
	while (split[++i])
	{
		temp = split[i];
		split[i] = ft_strjoin(temp, "/");
	}
	return (split);
}
