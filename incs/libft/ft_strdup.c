/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 11:54:50 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/04/09 19:31:57 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*buffer;
	int		i;

	i = 0;
	if(!s)
		return(NULL);
	buffer = malloc(ft_strlen(s) + 1);
	if (buffer == NULL)
		return (NULL);
	while (s[i] != '\0')
	{
		buffer[i] = s[i];
		i++;
	}
	buffer[i] = '\0';
	return (buffer);
}
