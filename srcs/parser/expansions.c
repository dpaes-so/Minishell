/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:58:38 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/05/10 18:00:53 by dpaes-so         ###   ########.fr       */
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
			if ((*token).value[i] != '\0' && (*token).value[i] == quote)
				count++;
			i++;
			while ((*token).value[i] != '\0' && (*token).value[i] != quote)
				i++;
			if ((*token).value[i] != '\0' && (*token).value[i] == quote)
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
		if (temp[i] != '\0')
			i++;
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

int		count_add(char *value)
{
	int	dummy_len;
	int	count;

	dummy_len = 0;
	count = 0;
	if (value == NULL)
		return (1);
	skip_wspaces(&value);
	while(value && *value)
	{
		if (*value && ft_strchr(" \t\n\v\f\r", *value) == NULL)
			count++;
		while (*value && ft_strchr(" \t\n\v\f\r", *value) == NULL)
		{
			if (is_quote(&value, &dummy_len) == false)
				value++;
		}
		skip_wspaces(&value);
	}
	return (count);
}

// int		count_exp_tokens(t_token *tokens, int flag)
// {
// 	int	i;
// 	int	count;
	
// 	i = 0;
// 	count = 0;
// 	if (flag == 1)
// 	{
// 		while(tokens[i].type != T_NULL)
// 		{
// 			count += count_add(tokens[i].value);
// 			i++;
// 		}
// 	}
// 	else if (flag == 2)
// 	{
// 		while(tokens[i].type != T_NULL)
// 		{
// 			count++;
// 			i++;
// 		}
// 	}
// 	return (count);
// }

// void	process_token(char	*value, t_token *new_tokens, int *i)
// {
// 	int	j;
// 	int	dummy_len;

// 	dummy_len = 0;
// 	j = 0;
// 	while(value && *value)
// 	{			
// 		while (value && ft_strchr(" \t\n\v\f\r", *value) == NULL)
// 		{
// 			// if (is_quote(&value, &dummy_len) == false)
// 			// 	value++;
// 		}
// 		skip_wspaces(&value);
// 	}
// }

// t_token *new_tokens(t_token *tokens)
// {
// 	int	i;
// 	int	j;
// 	int	amount;
// 	int	real_amount;
// 	t_token	*new_token;

// 	j = 0;
// 	i = 0;
// 	real_amount = count_exp_tokens(tokens, 2);
// 	amount = count_exp_tokens(tokens, 1);
// 	if (real_amount == amount)
// 		return (NULL);
// 	new_token = ft_calloc(amount + 1, sizeof(t_token));
// 	if (new_token == NULL)
// 		return (NULL);
// 	while (tokens[j].type != T_NULL)
// 	{
// 		if (count_add(tokens) > 1)
// 			process_token(tokens[j].value, new_tokens, &i);
// 		else 
// 			new_token[i].value = ft_strdup(tokens[j].value);
// 		new_token[i].type = tokens[j].type;
// 		i++;
// 		j++;
// 	}
// 	while
// }

void	expand_strs(t_token *tokens, t_mini *shell)
{
	int	i;

	i = 0;
	while (tokens[i].type != T_NULL)
	{
		if (tokens[i].type != T_PIPE)
		{
			if (tokens[i].type != T_HERE_DOC)
				dollar_expand(&tokens[i], shell);
		}
		i++;
	}
	// printf("exp_tokens = %d\n", count_exp_tokens(tokens, 1));
	i = 0;
	while (tokens[i].type != T_NULL)
	{
		if (tokens[i].type != T_PIPE)
			remove_quotes(&tokens[i]);
		i++;
	}
}
