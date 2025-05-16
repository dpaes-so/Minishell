/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:58:38 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/05/16 19:53:37 by dgarcez-         ###   ########.fr       */
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

// int	unclosed_split_quotes(char *value)
// {
// 	int		i;
// 	int		quote;
// 	char	cquote;

// 	i = 0;
// 	quote = 0;
// 	while (value[i])
// 	{
// 		if (value[i] == '\"' || value[i] == '\'')
// 		{
// 			quote++;
// 			cquote = value[i];
// 			i++;
// 			while (value[i] && value[i] != cquote)
// 				i++;
// 			if (value[i] == cquote)
// 			{
// 				quote++;
// 				i++;
// 			}
// 		}
// 		else
// 			i++;
// 	}
// 	return (quote);
// }

int	new_tokens_amount(t_token *tokens, int i, int j)
{
	char	**res;
	int		amount;

	amount = 0;
	while (tokens[i].type != T_NULL)
	{
		if (!(tokens[i].type >= T_HERE_DOC && tokens[i].type <= T_APPEND_REDIR))
		{
			j = 0;
			res = ft_arg_split(tokens[i].value, ' ');
			if (res == NULL)
				amount++;
			while (res && res[j])
				j++;
			amount += j;
			i++;
			freetrix(res);
		}
		else
		{
			amount++;
			i++;
		}
	}
	return (amount);
}

void	put_new_tokens(t_token *tokens, t_token *new_tokens, int *k, int *i)
{
	char	**res;
	int		j;

	j = -1;
	res = ft_arg_split(tokens[*i].value, ' ');
	while (res && res[++j])
	{
		new_tokens[*k].value = ft_strdup(res[j]);
		if (tokens[*i].type == T_PIPE)
			new_tokens[*k].type = token_type(new_tokens[*k].value, 1);
		else
			new_tokens[*k].type = token_type(new_tokens[*k].value, 0);
		(*k)++;
	}
	(*i)++;
	freetrix(res);
}

t_token	*create_new_tokens(t_token *tokens, int amount, int i, int k)
{
	t_token	*new_tokens;

	new_tokens = ft_calloc(amount + 1, sizeof(t_token));
	if (new_tokens == NULL)
		return (NULL);
	new_tokens[amount].type = T_NULL;
	new_tokens[amount].value = NULL;
	while (tokens[i].type != T_NULL)
	{
		if (!(tokens[i].type >= T_HERE_DOC && tokens[i].type <= T_APPEND_REDIR))
			put_new_tokens(tokens, new_tokens, &k, &i);
		else
		{
			new_tokens[k].value = ft_strdup(tokens[i].value);
			if (tokens[i].type == T_PIPE)
				new_tokens[k].type = token_type(new_tokens[k].value, 1);
			else
				new_tokens[k].type = token_type(new_tokens[k].value, 0);
			k++;
			i++;
		}
	}
	return (new_tokens);
}

t_token	*expand_strs(t_token *tokens, t_mini *shell)
{
	int		i;
	int		amount;
	t_token	*new_tokens;

	i = 0;
	amount = 0;
	while (tokens[i].type != T_NULL)
	{
		if (tokens[i].type != T_PIPE)
		{
			if (tokens[i].type != T_HERE_DOC)
				dollar_expand(&tokens[i], shell);
		}
		i++;
	}
	i = 0;
	amount = new_tokens_amount(tokens, 0, 0);
	new_tokens = create_new_tokens(tokens, amount, 0, 0);
	i = 0;
	while (new_tokens[i].type != T_NULL)
	{
		printf("new tokens[%d] = %s\n", i, new_tokens[i].value);
		printf("new tokens type [%d] = %d\n", i, new_tokens[i].type);
		i++;
	}
	i = 0;
	while (new_tokens[i].type != T_NULL)
	{
		if (new_tokens[i].type != T_PIPE)
			remove_quotes(&new_tokens[i]);
		i++;
	}
	return (new_tokens);
}
