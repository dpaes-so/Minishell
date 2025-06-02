/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_you_know_it_more_export_utils.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:08:52 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/06/02 15:11:33 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

int	check_char(char *s, int *ctd, int i)
{
	if (!((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')
			|| (s[i] >= '0' && s[i] <= '9') || (s[i] == '_') || s[i] == '+'
			|| s[i] == '=') || ((*ctd) == 1 && s[i] != '='))
		return (0);
	if (s[i] == '+')
		(*ctd)++;
	if ((*ctd) == 2)
		return (0);
	return (1);
}

void	*finish_on_god(char **new_export, char *arg, t_mini *mini, int size)
{
	new_export[size] = ft_strdup(arg);
	if (!new_export[size])
	{
		while (--size >= 0)
			free(new_export[size]);
		free(new_export);
		fmalloc(mini, "finish_fr", 2);
	}
	new_export[++size] = NULL;
	freetrix(mini->env->my_export);
	mini->env->my_export = new_export;
	return (NULL);
}
