/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstrf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 14:25:35 by dpaes-so          #+#    #+#             */
/*   Updated: 2024/11/04 14:34:00 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_fdprintf.h"

void	ft_dputstrf(const char *str, int *counter,int fd)
{
	int	i;
	int ret;

	i = 0;
	if (!str)
	{
		ret = write(fd, "(null)", 6);
		fd_derror(ret);
		(*counter) += 6;
		return ;
	}
	while (str[i] != '\0')
	{
		ret = write(fd, &str[i], 1);
		fd_derror(ret);
		i++;
		(*counter)++;
	}
}
