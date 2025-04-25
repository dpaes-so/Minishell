/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_aux.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:45:21 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/04/24 18:46:57 by dgarcez-         ###   ########.fr       */
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
		// printf("i = %d\n",i);
		free(matrix[i]);
		i++;
	}
	if (matrix)
		free(matrix);
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