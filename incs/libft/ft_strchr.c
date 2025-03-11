/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 11:41:23 by dpaes-so          #+#    #+#             */
/*   Updated: 2024/10/31 13:33:13 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int c)
{
	int		i;
	char	*s;

	i = 0;
	s = (char *)str;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
		{
			return (s + i);
		}
		i++;
	}
	if ((char)c == '\0')
		return (s + i);
	return (NULL);
}

/*int main()
{
	char *s = "hello world";
	char c = 'l';
	char *res = ft_strchr(s,c);
	printf("%s\n",res);
}*/