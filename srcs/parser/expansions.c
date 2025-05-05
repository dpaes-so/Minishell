/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:58:38 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/05/05 20:20:10 by dgarcez-         ###   ########.fr       */
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
	int		count;
	char	*temp;

	temp = ft_strdup((*token).value);
	if (temp == NULL)
		return (false);
	count = amount_quotes(token);
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

static int	countwords(const char *s)
{
	int	i;
	int	count;
	char	quote;

	quote = 0;
	i = 0;
	count = 0;
	while (s && s[i])
	{
		while (s[i] == ' ' && s[i])
			i++;
		if (s[i] == '\'' || s[i] == '\"')
		{
			quote = s[i];
			i++;
			count++;
			while (s[i] && s[i] != quote)
				i++;
			if (s[i] == quote)
				i++;
		}
		else if (s[i])
			count++;
		while (s[i] != ' ' && s[i])
			i++;
	}
	return (count);
}

void	count_exp(t_token *tokens)
{
	int	i;
	// int	count;

	i = 0;
	while(tokens[i].type != T_NULL)
	{
		printf("count words= %d\n", countwords(tokens[i].value));
		i++;
	}
}

bool	check_exp(t_token tokens)
{
	int	i;

	i = 0;
	ft_printf("gyasddasasyga = %s\n",tokens.value);
	while(tokens.value[i])
	{
		while (tokens.value[i] == ' ' && tokens.value[i])
			i++;
		if (tokens.value[i] == '\'')
		{
			i++;
			while (tokens.value[i] && tokens.value[i] != '\"')
				i++;
			if (tokens.value[i] == '\'')
				i++;
		}
		else
			i++;
		if(tokens.value[i] && tokens.value[i] == '$')
			return(true);
	}
	return (false);
}

t_token	*expand_strs(t_token *tokens, t_mini *shell)
{
	int	i;
	// t_token *new_tokens;

	i = 0;
	while (tokens[i].type != T_NULL)
	{
		if (tokens[i].type != T_PIPE)
		{
			if (tokens[i].type != T_HERE_DOC)
			{
				if(check_exp(tokens[i])== true)
				{
					dollar_expand(&tokens[i], shell);
					i++;
					continue;
				}
			}
			remove_quotes(&tokens[i]);
		}
		i++;
	}
	count_exp(tokens);
	return (tokens);
}
