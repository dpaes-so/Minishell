/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_matrix_dup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 17:06:49 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/12 17:19:09 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_matrix_dup(char **new_matrix, char **src)
{
	int	j;

	j = 0;
	while (src[j])
	{
		new_matrix[j] = ft_strdup(src[j]);
		if (!new_matrix[j])
		{
			while (j-- > 0)
				free(new_matrix[j]);
			return (NULL);
		}
		j++;
	}
	new_matrix[j] = NULL;
	return (new_matrix);
}