/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:42:40 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/04/16 17:38:26 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

void	build_exit(t_mini *mini)
{
	free(mini->input);
	if (mini->pwd)
		free(mini->pwd);
	free(mini->env->home);
	freetrix(mini->env->my_env);
	free(mini->env);
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

int	build_unset(t_mini *mini)
{
	int	i;

	i = -1;
	while (mini->env->my_env[++i])
		if (ft_strncmp(mini->env->my_env[i], mini->input + 6,
				ft_strlen(mini->input + 6)) == 0
			&& mini->env->my_env[i][ft_strlen(mini->input + 6)] == '=')
			break ;
	// printf("UNSETTING SOMETHING BITCH\n%s\n", mini->env->my_env[i]);
	while (mini->env->my_env[i] != NULL)
	{
		free(mini->env->my_env[i]);
		mini->env->my_env[i] = ft_strdup(mini->env->my_env[i + 1]);
		i++;
	}
	return (1);
}

int	build_pwd(t_mini *mini)
{
	ft_printf("%s\n", mini->pwd);
	return (1);
}
