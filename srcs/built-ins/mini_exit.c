/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 17:39:46 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/20 20:05:01 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

static void	*check_exit_code(t_mini *mini, t_cmd cmds, int *f)
{
	int	j;

	j = -1;
	if (!cmds.args[1][0])
	{
		ft_putstr_fd("Minishell: exit: Numerical input required\n", 2);
		mini->pipex.status = 2;
		return (*f = 1, NULL);
	}
	if (cmds.args[1][0] == '-')
		j++;
	while (cmds.args[1][++j])
	{
		if (j == 0 && (cmds.args[1][0] == '+' || cmds.args[1][0] == '-'))
			j++;
		else if (!((cmds.args[1][j] >= '0' && cmds.args[1][j] <= '9')
				|| (cmds.args[1][j] >= 9 && cmds.args[1][j] <= 13)
				|| cmds.args[1][j] == ' ') || *f == 1)
		{
			ft_putstr_fd("Minishell: exit: Numerical input required\n", 2);
			mini->pipex.status = 2;
			return (*f = 1, NULL);
		}
	}
	return (NULL);
}

int	build_exit(t_mini *mini, t_cmd cmds)
{
	long long	n;
	int			f;

	f = 0;
	if (mini->cmd_amount == 1)
		mini->wait_check = 0;
	if (do_redirect(&cmds, mini) < 0)
		return (mini->pipex.status = 1, 1);
	if (cmds.args[1])
	{
		n = ft_atol(cmds.args[1], &f);
		check_exit_code(mini, cmds, &f);
	}
	if (cmds.amount != 1 && f != 1)
		mini->pipex.status = (unsigned char)n;
	free(mini->pwd);
	if (mini->env->home != NULL)
		free(mini->env->home);
	freetrix(mini->env->my_env);
	freetrix(mini->env->my_export);
	free(mini->env);
	free_tree(mini->ast);
	freetrix(mini->pipex.path);
	clear_history();
	master_close();
	ft_dprintf(2,"exit\n");
	if (cmds.amount > 2)
	{
		ft_dprintf(2,"too many arguments\n");
		exit(1);
	}
	exit(mini->pipex.status);
}
