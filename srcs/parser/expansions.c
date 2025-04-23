/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:58:38 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/04/23 16:06:31 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

void	remove_s_quotes(t_token token)
{
	int		i;
	int		j;
	int		count;
	char	*temp;

	temp = ft_strdup(token.value);
	if (temp == NULL)
		return;
	count = 0;
	i = 0;
	j = 0;
	while(token.value[i])
	{
		if(token.value[i] == '\'')
		{
			i++;
			while(token.value[i] != '\0' && token.value[i] != '\'')
				i++;
			if (token.value[i] != '\0' && token.value[i] == '\'')
				count += 2;
		}
		i++;
	}
	free(token.value);
	token.value = ft_calloc((ft_strlen(temp) - count) + 1, sizeof(char));
	if (token.value == NULL)
		return ;
	i = 0;
	while(temp[i])
	{
		if(temp[i] == '\'')
		{
			i++;
			while(temp[i] != '\0' && temp[i] != '\'')
			{
				token.value[j] = temp[i];
				j++;
				i++;
			}
		}
		else
		{
			token.value[j] = temp[i];
			j++;
		}
		i++;
	}
	token.value[ft_strlen(temp) - count] = '\0';
	free(temp);
}

void	expand_strs(t_token *tokens)
{
	int	i;
	
	i = 0;
	while(tokens[i].type != T_NULL)
	{
		if (tokens[i].type != T_PIPE)
		{
			remove_s_quotes(tokens[i]);
		}
		i++;
	}
}