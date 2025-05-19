/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putcharf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 14:25:11 by dpaes-so          #+#    #+#             */
/*   Updated: 2024/11/04 14:25:11 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_fdprintf.h"

void fd_derror(int ret)
{
	if(ret < 0)
	{
		write(2,"Error, invalid fd\n",19);
		exit(1);
	}
}

void	ft_dputcharf(const char c, int *counter,int fd)
{
	int ret;

	ret = write(fd, &c, 1);
	fd_derror(ret);
	(*counter)++;
}
