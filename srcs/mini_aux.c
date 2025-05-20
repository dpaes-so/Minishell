/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_aux.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:58:23 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/20 19:42:23 by dpaes-so         ###   ########.fr       */
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

void	exit_childprocess(t_mini *mini, int ecode)
{
	if (mini->pwd)
		free(mini->pwd);
	if(mini->env)
	{
		if (mini->env->home != NULL)
			free(mini->env->home);
		if (mini->env->my_env)
			freetrix(mini->env->my_env);
		if (mini->env->my_export)
			freetrix(mini->env->my_export);
		if (mini->env)
			free(mini->env);
	}
	if (ecode != -2)
	{
		if (mini->ast)
			free_tree(mini->ast);
	}
	else
		ecode = 0;
	if (mini->pipex.path)
		freetrix(mini->pipex.path);
	clear_history();
	master_close();
	exit(ecode);
}

void	exit_childprocess_exec(t_mini *mini)
{
	if (mini->pwd)
		free(mini->pwd);
	if (mini->env->home && mini->env->home != NULL)
		free(mini->env->home);
	if (mini->env->my_env)
		freetrix(mini->env->my_env);
	if (mini->env->my_export)
		freetrix(mini->env->my_export);
	if (mini->env)
		free(mini->env);
	if (mini->ast)
		free_tree(mini->ast);
	if (mini->pipex.path)
		free(mini->pipex.path);
	clear_history();
	master_close();
}

char	**path_finder(char **envp)
{
	int		i;
	char	*temp;
	char	*str;
	char	**split;

	str = NULL;
	if(!envp)
		return(NULL);
	i = -1;
	while (envp && envp[++i])
		if (ft_strnstr(envp[i], "PATH", 4))
			break ;
	if (!envp[i])
		return (NULL);
	str = envp[i] + 5;
	split = ft_split(str, ':');
	i = -1;
	while (split && split[++i])
	{
		temp = split[i];
		split[i] = ft_strjoin(temp, "/");
		if(!split[i])
			return(freetrix(split),NULL);
	}
	return (split);
}
