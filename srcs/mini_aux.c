/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_aux.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:45:21 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/04/29 16:14:52 by dgarcez-         ###   ########.fr       */

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
		// ft_printf("i = %d\n",i);
		free(matrix[i]);
		i++;
	}
	if (matrix)
		free(matrix);
	// ft_printf("ACABOU\n");
}
void master_close()
{
    int i;

    i = 2;
    while(++i < 1024)
        close(i);
}

void exit_childprocess(t_mini *mini)
{
	free(mini->pwd);
	if(mini->env->home != NULL)
		free(mini->env->home);
	freetrix(mini->env->my_env);
	free(mini->env);
	free_tree(mini->ast);
	clear_history();
	master_close();
	exit(0);
}

char	**path_finder(char **envp, t_pipe pipe)
{
	int		i;
	char	*temp;
	char	**split;
	char *temp_ev;

	i = -1;
	while (envp[++i])
		if (ft_strnstr(envp[i], "PATH", 4))
			break ;
	if (!envp[i])
	{
		close(pipe.outfile_fd);
		exit(0);
	}
	temp_ev = envp[i] + 5;
	split = ft_split(temp_ev, ':');
	i = -1;
	while (split[++i])
	{
		temp = split[i];
		split[i] = ft_strjoin(temp, "/");
		// free(temp);
	}
	return (split);
}
