/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 16:51:17 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/12 18:08:36 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

int	print_env_ex(t_mini *mini)
{
	int	i;

	i = -1;
	while (mini->env->my_env[++i])
		ft_printf("declare -x %s\n", mini->env->my_env[i]);
	return (1);
}

void	*add_export(t_mini *mini, char *arg)
{
	char	**new_env;
	int		size;
	int		break_point;
	int		i;

	size = 0;
	i = -1;
	break_point = -1;
	while (mini->env->my_env[size])
		size++;
	while (mini->env->my_env[++break_point])
		if (!ft_strncmp(mini->env->my_env[break_point], arg, ft_strchr(arg, '+')
				- arg))
			return (mini->env->my_env[break_point] = ft_strjoin(mini->env->my_env[break_point],
					ft_strchr(arg, '+') + 2), NULL);
	new_env = ft_calloc((size + 2), sizeof(char *));
	if (!new_env)
		return (NULL);
	while (mini->env->my_env[++i])
		new_env[i] = ft_strdup(mini->env->my_env[i]);
	new_env[i++] = ft_strdup(arg);
	new_env[i] = NULL;
	freetrix(mini->env->my_env);
	return (mini->env->my_env = new_env, NULL);
}
