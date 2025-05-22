/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniel <daniel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:33:45 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/05/21 02:21:58 by daniel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

void	small_cpy(t_token *token, char *expand, int *j, int *amount)
{
	if (expand != NULL)
	{
		expand[*j] = *(*token).value;
		(*j)++;
	}
	if (expand == NULL)
		(*amount)++;
	(*token).value++;
}

char	*found_dollar(t_token *token, t_mini *shell, int *flag)
{
	char	*expand;

	expand = NULL;
	if (*(*token).value == '$')
	{
		(*token).value++;
		if (*(*token).value == '?')
		{
			expand = status_expand(shell);
			if (expand == NULL)
				return (NULL);
			*flag = 1;
			(*token).value++;
		}
		else if (ft_isalnum(*(*token).value) || *(*token).value == '_')
		{
			expand = find_env(token, shell);
			if (expand == NULL)
				return (NULL);
		}
		else
			return (no_dollar(*(token)));
	}
	return (expand);
}

void	handle_s_quote(t_token *token, char *expand, int *j)
{
	expand[*j] = *(*token).value;
	(*j)++;
	(*token).value++;
	while (*(*token).value && *(*token).value != '\'')
	{
		expand[*j] = *(*token).value;
		(*j)++;
		(*token).value++;
	}
	expand[*j] = *(*token).value;
	(*j)++;
	(*token).value++;
}

void	d_quote_helper(t_token *token, t_mini *shell, char *expand, int *j)
{
	if (expand != NULL)
	{
		small_cpy(token, expand, j, 0);
		while (*(*token).value && *(*token).value != '\"')
		{
			if (*(*token).value && *(*token).value == '$')
				handle_dollar(token, shell, expand, j);
			else
				small_cpy(token, expand, j, 0);
		}
		small_cpy(token, expand, j, 0);
	}
	else
	{
		small_cpy(token, NULL, 0, j);
		while (*(*token).value && *(*token).value != '\"')
		{
			if (*(*token).value && *(*token).value == '$')
				handle_dollar(token, shell, NULL, j);
			else
				small_cpy(token, NULL, 0, j);
		}
		small_cpy(token, NULL, 0, j);
	}
}

void	handle_d_quote(t_token *token, t_mini *shell, char *expand, int *j)
{
	(*token).in_quotes = true;
	d_quote_helper(token, shell, expand, j);
	(*token).in_quotes = false;
}
