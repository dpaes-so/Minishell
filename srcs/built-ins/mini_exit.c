/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 17:39:46 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/06/02 18:21:32 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

void	omega_free(t_mini *mini)
{
	free(mini->pwd);
	if (mini->env && mini->env->my_env)
		freetrix(mini->env->my_env);
	if (mini->env && mini->env->my_env)
		freetrix(mini->env->my_export);
	if (mini->env)
		free(mini->env);
	free_tree(mini->ast);
	if (mini->pipex.path)
		freetrix(mini->pipex.path);
}

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
	while (cmds.args[1][++j])
	{
		if (j == 0 && (cmds.args[1][0] == '+' || cmds.args[1][0] == '-')
			&& cmds.args[1][1] != '\0')
			continue ;
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
	if (cmds.amount > 2 && f == 0)
		return (ft_dprintf(2, "exit\n"), mini->pipex.status = 1, ft_dprintf(2,
				"too many arguments\n"), 1);
	clear_history();
	master_close();
	omega_free(mini);
	if (mini->cmd_amount == 1)
		ft_dprintf(1, "exit\n");
	exit(mini->pipex.status);
}
