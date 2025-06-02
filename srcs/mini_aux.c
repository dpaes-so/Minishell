/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_aux.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:58:23 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/06/02 14:30:41 by dpaes-so         ###   ########.fr       */
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
	free(matrix);
}

void	master_close(void)
{
	int	i;

	i = 2;
	while (++i < 1024)
		close(i);
}

char 	**path_add(t_mini *mini,char **split)
{
	int i;
	int size;
	char **fres;

	size = 0;
	while(split[size])
		size++;
	fres = ft_calloc(size + 1,sizeof(char *));
	if(!fres)
	{
		freetrix(split);
		fmalloc(mini, "path_add", 2);
	}
	i = -1;
	while (split && split[++i])
	{
		fres[i] = ft_strjoin(split[i], "/");
		if (!fres[i])
		{
			while(++i < size)
				free(split[i]);
			free(split);
			return (mini->f_malloc = 1, freetrix(fres), NULL);
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
		fmalloc(mini, "path_finder", 2);
	return (path_add(mini, split));
}
