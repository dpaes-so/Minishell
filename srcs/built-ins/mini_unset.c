/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniel <daniel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:27:22 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/18 22:27:34 by daniel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"


static void do_unset(t_mini *mini, t_cmd cmds,int cmd_n,int i)
{
	while (mini->env->my_env[++i])
		if (ft_strncmp(mini->env->my_env[i], cmds.args[cmd_n],
				ft_strlen(cmds.args[cmd_n])) == 0
			&& mini->env->my_env[i][ft_strlen(cmds.args[cmd_n])] == '=')
			break ;
	while (mini->env->my_env[i] != NULL)
	{
		free(mini->env->my_env[i]);
		mini->env->my_env[i] = ft_strdup(mini->env->my_env[i + 1]);
		i++;
	}
	i = -1;
	while (mini->env->my_export[++i])
		if (ft_strncmp(mini->env->my_export[i], cmds.args[cmd_n],
				ft_strlen(cmds.args[cmd_n])) == 0
			&& mini->env->my_export[i][ft_strlen(cmds.args[cmd_n])] == '=')
			break ;
	while (mini->env->my_export[i] != NULL)
	{
		free(mini->env->my_export[i]);
		mini->env->my_export[i] = ft_strdup(mini->env->my_export[i + 1]);
		i++;
	}
}
int	build_unset(t_mini *mini, t_cmd cmds)
{
	int	cmd_n;
	int i;

	i = -1;
	cmd_n = 0;
	if(mini->cmd_amount == 1)
		mini->wait_check = 0;
	if(do_redirect(&cmds, mini) < 0)
		return(mini->pipex.status = 1,1);
	if (!cmds.args[1])
		return (1);
	while (cmds.args[++cmd_n])
		do_unset(mini,cmds,cmd_n,i);
	return (1);
}
