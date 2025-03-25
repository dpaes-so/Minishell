/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:51:59 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/03/25 18:59:55 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while ((s1[i] != '\0' || s2[i] != '\0') )
	{
		if (!(s1[i] == s2[i]))
			return ((unsigned char)s1[i] - ((unsigned char )s2[i]));
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
// int main()
// {
// 	char *s1 ="cd";
// 	char *s2 = "cd";
// 	printf("blA %d",ft_strcmp(s1,s2));
// }