/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:26:48 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/12 17:27:20 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

static void	check_exit_code(t_mini *mini, t_cmd cmds, int f)
{
	int	j;

	j = -1;
	while (cmds.args[1][++j])
	{
		if (!((cmds.args[1][j] >= '0' && cmds.args[1][j] <= '9')
				|| (cmds.args[1][j] >= 9 && cmds.args[1][j] <= 13)
				|| cmds.args[1][j] >= ' ') || f == 1)
		if (!(cmds.args[1][j] >= '0' && cmds.args[1][j] <= '9')
			&& cmds.args[1][j] != ' ' && cmds.args[1][j] != '+')
		{
			ft_putstr_fd("Minishell: exit: Numerical input required\n", 2);
			mini->pipex.status = 255;
			exit(mini->pipex.status);
		}
	}
}

int	build_exit(t_mini *mini, t_cmd cmds)
{
	long long	n;

	do_redirect(&cmds, mini);
	if (cmds.amount != 0)
		mini->pipex.status = ft_atoi(cmds.args[0]);
	if (cmds.args[1])
		check_exit_code(mini, cmds);
	if (cmds.amount > 2)
		return (ft_putstr_fd("Minishell: exit: too many argumetns\n", 2), 1);
	free(mini->pwd);
	if (mini->env->home != NULL)
		free(mini->env->home);
	freetrix(mini->env->my_env);
	free(mini->env);
	free_tree(mini->ast);
	freetrix(mini->pipex.path);
	clear_history();
	master_close();
	exit(mini->pipex.status);
}
