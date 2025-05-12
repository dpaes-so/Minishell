/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 14:30:24 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/12 16:23:35 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long long	ft_atol(const char *nptr)
{
	int			i;
	int			minus;
	long long	result;

	i = 0;
	minus = 1;
	result = 0;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			minus = -minus;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9' && result < LONG_MAX)
	{
		if (result  >= LONG_MAX / 10)
			break ;
		printf("result = %lld\n",result);
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	return (result * minus);
}
