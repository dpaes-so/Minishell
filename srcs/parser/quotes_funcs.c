/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_funcs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 00:44:11 by daniel            #+#    #+#             */
/*   Updated: 2025/06/04 13:59:31 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

int	amount_quotes(t_token *token)
{
	char	quote;
	int		count;
	int		i;

	count = 0;
	i = 0;
	while ((*token).value[i])
	{
		if ((*token).value[i] == '\'' || (*token).value[i] == '\"')
		{
			quote = (*token).value[i];
			count++;
			i++;
			while ((*token).value[i] && (*token).value[i] != quote)
				i++;
			if ((*token).value[i] == quote)
			{
				count++;
				i++;
			}
		}
		else
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
	i = 0;
	while (temp[i])
	{
		if (temp[i] == '\'' || temp[i] == '\"')
		{
			quote = temp[i++];
			while (temp[i] != '\0' && temp[i] != quote)
			{
				(*token).value[j++] = temp[i];
				i++;
			}
		}
		else
		{
			(*token).value[j] = temp[i];
			j++;
		}
		if (temp[i] != '\0')
			i++;
	}
}

char	*put_quotes(char *temp, int count, t_mini *shell, int *flag)
{
	int		i;
	int		j;
	char	*new_expand;

	i = 0;
	j = 0;
	new_expand = ft_calloc(ft_strlen(temp) + (count * 2) + 1, sizeof(char));
	if (new_expand == NULL)
		return (set_fmalloc(shell));
	*flag = 2;
	while (temp[i])
	{
		if (temp[i] == '\'')
			quoting_quotes(new_expand, &j, '\'', '\"');
		else if (temp[i] == '\"')
			quoting_quotes(new_expand, &j, '\"', '\'');
		else
			new_expand[j++] = temp[i];
		i++;
	}
	return (new_expand);
}

char	*add_quotes(char *temp, int *flag, t_mini *shell)
{
	int		i;
	int		count;

	count = 0;
	i = 0;
	while (temp[i])
	{
		if (temp[i] == '\'' || temp[i] == '\"')
			count++;
		i++;
	}
	if (count == 0)
		return (temp);
	temp = put_quotes(temp, count, shell, flag);
	return (temp);
}

bool	remove_quotes(t_token *token, t_mini *shell)
{
	int		count;
	char	*temp;

	temp = ft_strdup((*token).value);
	if (temp == NULL)
		return (false);
	count = amount_quotes(token);
	if (count == 0)
	{
		free (temp);
		return (false);
	}
	if ((*token).type == T_HERE_DOC && count > 0)
		(*token).in_quotes = true;
	free((*token).value);
	(*token).value = ft_calloc((ft_strlen(temp) - count) + 1, sizeof(char));
	if ((*token).value == NULL)
	{
		free(temp);
		return (shell->f_malloc = 1, false);
	}
	(*token).value[ft_strlen(temp) - count] = '\0';
	removed(token, temp);
	free(temp);
	return (true);
}
