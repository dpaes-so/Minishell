/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:27:22 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/05 12:27:23 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

int	build_unset(t_mini *mini, t_cmd cmds)
{
	int	i;
	int	cmd_n;

	cmd_n = 0;
	if(mini->cmd_amount == 1)
		mini->wait_check = 0;
	if(do_redirect(&cmds, mini))
		return(mini->pipex.status = 1,1);
	if (!cmds.args[1])
		return (0);
	while (cmds.args[++cmd_n])
	{
		i = -1;
		while (mini->env->my_env[++i])
			if (ft_strncmp(mini->env->my_env[i], cmds.args[cmd_n],
					ft_strlen(cmds.args[cmd_n])) == 0
				&& mini->env->my_env[i][ft_strlen(cmds.args[cmd_n])] == '=')
				break ;
		printf("UNSETTING SOMETHING BITCH\n%s\n", mini->env->my_env[i]);
		while (mini->env->my_env[i] != NULL)
		{
			free(mini->env->my_env[i]);
			mini->env->my_env[i] = ft_strdup(mini->env->my_env[i + 1]);
			i++;
		}
	}
	return (1);
}
