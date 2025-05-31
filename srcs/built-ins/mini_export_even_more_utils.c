/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export_even_more_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:02:57 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/30 18:28:21 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

int	print_env_ex(t_mini *mini)
{
	int	i;

	i = -1;
	ft_sort_matrix(mini->env->my_export);
	while (mini->env->my_export && mini->env->my_export[++i])
		ft_printf("declare -x %s\n", mini->env->my_export[i]);
	return (1);
}

char	*remove_plus_sign(char *src)
{
	char	*new_arg;
	int		i;
	int		j;

	j = 0;
	i = 0;
	new_arg = ft_calloc(ft_strlen(src), sizeof(char));
	if (new_arg == NULL)
		return (NULL);
	while (src[j])
	{
		if (src[j] != '+')
		{
			new_arg[i] = src[j];
			i++;
		}
		j++;
	}
	new_arg[i] = '\0';
	return (new_arg);
}

int	check_valid_variable_name(char *s)
{
	int	i;
	int	ctd;

	i = 0;
	ctd = 0;
	if (s[0] == '=')
		return (0);
	if (!((s[0] >= 'a' && s[0] <= 'z') || (s[0] >= 'A' && s[0] <= 'Z')
			|| (s[0] == '_')))
		return (0);
	while (s[i] && s[i] != '=')
	{
		if (!check_char(s, &ctd, i))
			return (0);
		i++;
	}
	if (ctd == 1 && s[i] != '=')
		return (0);
	if (s[i - 1] == '+')
		return (2);
	return (1);
}
