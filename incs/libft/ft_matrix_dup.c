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

char	**ft_matrix_dup(char **new_matrix, char **matrixdup)
{
	int	j;

	j = -1;
	while (matrixdup[++j])
	{
		new_matrix[j] = ft_strdup(matrixdup[j]);
		if (new_matrix[j] == NULL)
			return (NULL);
	}
	return (new_matrix);
}