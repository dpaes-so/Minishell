/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:58:38 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/05/15 12:09:31 by dgarcez-         ###   ########.fr       */
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

int	unclosed_split_quotes(char *value)
{
	int		i;
	int		quote;
	char	cquote;

	i = 0;
	quote = 0;
	while (value[i])
	{
		if (value[i] == '\"' || value[i] == '\'')
		{
			quote++;
			cquote = value[i];
			i++;
			while (value[i] && value[i] != cquote)
				i++;
			if (value[i] == cquote)
			{
				quote++;
				i++;
			}
		}
		else
			i++;
	}
	return (quote);
}

char	*no_split_dollar(char *token, int i)
{
	int	count;

	count = unclosed_split_quotes(token + i);
	if (count % 2 != 0 || count == 0)
		return ("$");
	return (NULL);
}

char	*found_split_dollar(char *copy, int *i, t_mini *shell)
{
	char	*expand;

	expand = NULL;
	if (copy[*i] == '$')
	{
		(*i)++;
		if (copy[*i] == '?')
		{
			expand = status_expand(shell);
			if (expand == NULL)
				return (NULL);
			(*i)++;
		}
		else if (ft_isalnum(copy[*i]) || copy[*i] == '_')
		{
			expand = find_in_env(copy + *i, shell);
			while (copy[*i] && (ft_isalnum(copy[*i]) || copy[*i] == '_'))
				(*i)++;
			if (expand == NULL)
				return (NULL);
		}
		else
			return (no_split_dollar(copy, *i));
	}
	return (expand);
}

static int	countwords(const char *s, int *f)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	printf("count string = %s\n", s);
	while (s[i])
	{
		while (s[i] && ft_strchr(" \t\n\v\f\r", s[i]) != NULL)
		{
			if (*f == 2)
				count++;
			*f = 0;
			i++;
		}
		if (s[i])
			count++;
		while (s[i] && ft_strchr(" \t\n\v\f\r", s[i]) == NULL)
		{
			*f = 1;
			i++;
		}
	}
	return (count);
}

int	handle_split_dollar(char *copy, int *i, t_mini *shell, bool quotes)
{
	int		count;
	char	*temp;
	int		flag;

	count	= 0;
	if (*i > 0)
		flag = 2;
	else
		flag = 0;
	temp = found_split_dollar(copy, i, shell);
	if (temp != NULL && temp[0])
	{
		if (!quotes)
		{
			count = countwords(temp, &flag);
			if (flag == 0 && copy[*i])
				count++;
			return (count);
		}
		else
			(*i)++;
	}
	return (0);
}

int	identifier(char *copy, int *i, t_mini *shell)
{
	if (copy[*i] == '\'')
	{
		(*i)++;
		while (copy[*i] && copy[*i] != '\'')
			(*i)++;
		(*i)++;
	}
	else
	{
		if (copy[*i] == '\"')
		{
			(*i)++;
			while (copy[*i] && copy[*i] != '\"')
			{
				if (copy[*i] && copy[*i] == '$')
					return (handle_split_dollar(copy, i, shell, true));
				else
					(*i)++;
			}
			(*i)++;
		}
		else if (copy[*i] == '$')
			return (handle_split_dollar(copy, i, shell, false));
		else
			(*i)++;
	}
	return (0);
}

int	count_add(char *copy, t_mini *shell)
{
	int	i;
	int	amount;

	amount = 0;
	i = 0;
	(void)shell;
	if (copy == NULL)
		return (1);
	while (copy[i])
	{
		printf("copy[%d] = %c\n",i, copy[i]);
		amount += identifier(copy, &i, shell);
	}
	return (amount);
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
	i = 0;
	printf("count_add amount dumbass = %d\n", count_add(tokens[i].copy, shell));
	while (tokens[i].type != T_NULL)
	{
		if (tokens[i].type != T_PIPE)
			remove_quotes(&tokens[i]);
		i++;
	}
}
