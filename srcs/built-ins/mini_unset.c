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

int	build_unset(t_mini *mini, t_cmd cmds)
{
	int	i;
	int	cmd_n;

	cmd_n = 0;
	do_redirect(&cmds, mini);
	if (!cmds.args[1])
		return (1);
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
