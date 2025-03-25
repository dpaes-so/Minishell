/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 11:56:13 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/03/25 15:32:04 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*buffer;
	int		i;
	int		j;

	if (!s1)
		return (NULL);
	i = 0;
	j = 0;
	buffer = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 2));
	if (!buffer)
		return (NULL);
	while (i < ft_strlen(s1))
	{
		buffer[i] = s1[i];
		i++;
	}
	while (j < ft_strlen(s2))
	{
		buffer[i + j] = s2[j];
		j++;
	}
	buffer[i + j] = '\0';
	return (buffer);
}

/*int main()
{
	char str[] = "hello, ";
	char str2[] = "world";
	char *res = ft_strjoin(str,str2);
	printf("%s\n",res);
}*/