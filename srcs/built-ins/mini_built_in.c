/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:42:40 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/04/16 17:40:48 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

void	build_exit(t_mini *mini,t_cmd cmds)
{
	(void)cmds;
	free(mini->pwd);
	free(mini->env->home);
	freetrix(mini->env->my_env);
	free(mini->env);
	free_tree(mini->ast);
	clear_history();
	exit(0);
}

void	build_echo(t_mini *mini)
{
	if (ft_strncmp(mini->input + 5, "-n ", 3) == 0)
		ft_printf("%s", mini->input + 8);
	else
		ft_printf("%s\n", mini->input + 5);
}

int	build_unset(t_mini *mini,t_cmd cmds)
{
	int	i;
	int cmd_n;

	cmd_n = -1;
	if (!cmds.args[0])
		return (0);
	while(cmds.args[++cmd_n])
	{
		i = -1;
		printf("IMA TRY TO UNSET THIS  \n%s\n", cmds.args[cmd_n]);
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

int	build_pwd(t_mini *mini)
{
	get_pwd(mini);
	ft_printf("%s\n", mini->pwd);
	return (1);
}
