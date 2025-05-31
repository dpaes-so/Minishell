/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_aux.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:58:23 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/30 19:00:41 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/mini_header.h"

void	freetrix(char **matrix)
{
	int	i;

	i = 0;
	if (!matrix)
		return ;
	ft_printf("FREE'D\n");
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
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
	if (mini->env)
	{
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
char 	**path_add(t_mini *mini,char **split)
{
	int i;
	char **fres;

	i = 0;
	while(split[i])
		i++;
	fres = ft_calloc(i + 1,sizeof(char *));
	i = -1;
	while (split && split[++i])
	{
		fres[i] = ft_strjoin(split[i], "/");
		if (!fres[i])
		{
			freetrix(split);
			return (mini->f_malloc = 1,freetrix(fres), NULL);
		}
	}
	free(split);
	return(fres);
}
char	**path_finder(char **envp, t_mini *mini)
{
	int		i;
	char	*str;
	char	**split;

	str = NULL;
	if (!envp)
		return (NULL);
	i = -1;
	while (envp && envp[++i])
		if (ft_strnstr(envp[i], "PATH", 4))
			break ;
	if (!envp[i])
		return (NULL);
	str = envp[i] + 5;
	split = ft_split(str, ':');
	if(!split)
		fmalloc(mini);
	return (path_add(mini,split));
}
