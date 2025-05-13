/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_more_exec_aux.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:33:17 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/13 17:34:21 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

char	**matrix_dup(t_mini *mini, char **ev)
{
	int	j;

	j = -1;
	while (ev[++j])
	{
		mini->env->my_env[j] = ft_strdup(ev[j]);
		if (mini->env->my_env[j] == NULL)
			return (NULL);
	}
	return (mini->env->my_env);
}

t_mini	*mem_save(t_mini *to_save)
{
	static t_mini	*save;

	if (to_save)
		save = to_save;
	return (save);
}
