/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:58:38 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/05/01 20:05:37 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

int	amount_quotes(t_token *token)
{
	char	quote;
	int		count;
	int		i;

	quote = 0;
	count = 0;
	i = 0;
	while ((*token).value[i])
	{
		if ((*token).value[i] == '\'' || (*token).value[i] == '\"')
		{
			quote = (*token).value[i];
			i++;
			while ((*token).value[i] != '\0' && (*token).value[i] != quote)
				i++;
			if ((*token).value[i] != '\0' && (*token).value[i] == quote)
				count += 2;
		}
		i++;
	}
	return (count);
}

void	removed(t_token *token, char *temp)
{
	char	quote;
	int		i;
	int		j;

	j = 0;
	i = -1;
	while (temp[++i])
	{
		if (temp[i] == '\'' || temp[i] == '\"')
		{
			quote = temp[i];
			i++;
			while (temp[i] != '\0' && temp[i] != quote)
			{
				(*token).value[j] = temp[i];
				j++;
				i++;
			}
		}
		else
		{
			(*token).value[j] = temp[i];
			j++;
		}
	}
}

bool	remove_quotes(t_token *token)
{
	int		i;
	int		j;
	int		count;
	char	*temp;

	temp = ft_strdup((*token).value);
	if (temp == NULL)
		return (false);
	count = amount_quotes(token);
	i = 0;
	j = 0;
	free((*token).value);
	(*token).value = ft_calloc((ft_strlen(temp) - count) + 1, sizeof(char));
	if ((*token).value == NULL)
		return (false);
	(*token).value[ft_strlen(temp) - count] = '\0';
	removed(token, temp);
	free(temp);
	if (count == 0)
		return (false);
	return (true);
}

void	expand_strs(t_token *tokens, t_mini shell)
{
	int	i;

	i = 0;
	while (tokens[i].type != T_NULL)
	{
		if (tokens[i].type != T_PIPE)
		{
			if (tokens[i].type != T_HERE_DOC)
				dollar_expand(&tokens[i], shell);
			remove_quotes(&tokens[i]);
		}
		i++;
	}
}
