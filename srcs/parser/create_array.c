/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_array.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:59:48 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/06/02 13:13:49 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

int	count_elems(t_token *tokens)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (tokens[i].type != T_NULL)
	{
		if (tokens[i].type != T_PIPE)
			count++;
		while (tokens[i].type != T_NULL && tokens[i].type != T_PIPE)
			i++;
		if (tokens[i].type != T_NULL && tokens[i].type == T_PIPE)
		{
			count++;
			i++;
		}
	}
	return (count);
}

int	token_count(t_token *tokens, int *i)
{
	int	amount;

	amount = 0;
	if (tokens[*i].type == T_PIPE)
	{
		(*i)++;
		return (1);
	}
	else
	{
		while (tokens[*i].type != T_NULL && tokens[*i].type != T_PIPE)
		{
			(*i)++;
			amount++;
		}
	}
	return (amount);
}

void	init_array(t_token **array, t_token *tokens)
{
	int	i;
	int	j;
	int	k;

	i = -1;
	k = 0;
	while (array && array[++i])
	{
		j = -1;
		if (array[i][++j].type != T_NULL && tokens[k].type == T_PIPE)
		{
			array[i][j].value = ft_strdup(tokens[k++].value);
			array[i]->type = T_PIPE;
		}
		else
		{
			while (array[i][j].type != T_NULL && array[i][j].type != T_PIPE)
			{
				array[i][j].value = ft_strdup(tokens[k].value);
				array[i][j].in_quotes = tokens[k].in_quotes;
				array[i][j].ambiguous = tokens[k].ambiguous;
				array[i][j++].type = tokens[k++].type;
			}
		}
	}
}

void	free_array(t_token **array)
{
	int	i;
	int	j;

	i = 0;
	while (array && array[i])
	{
		j = 0;
		while (array[i][j].type != T_NULL)
		{
			free(array[i][j].value);
			j++;
		}
		free(array[i]);
		i++;
	}
	free(array);
}

t_token	**array_creation(t_token *tokens, t_mini *shell)
{
	int		i;
	int		index;
	int		len;
	int		amount;
	t_token	**array;

	i = 0;
	index = 0;
	amount = count_elems(tokens);
	array = ft_calloc(amount + 1, sizeof(t_token *));
	if (array == NULL)
		return (shell->f_malloc = 1, NULL);
	array[amount] = NULL;
	while (i < amount)
	{
		len = token_count(tokens, &index);
		array[i] = ft_calloc(len + 1, sizeof(t_token));
		if (array[i] == NULL)
			return (free_array(array), shell->f_malloc = 1, NULL);
		array[i][len].value = NULL;
		array[i][len].type = T_NULL;
		i++;
	}
	return (array);
}
