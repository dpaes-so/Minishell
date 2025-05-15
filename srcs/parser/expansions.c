/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:58:38 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/05/15 18:36:29 by dgarcez-         ###   ########.fr       */
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

void	do_count_words_loop_sigma_best_ever_made(int *i, char *s, char quote,
		int *count)
{
	while (s[*i] && (s[*i] == ' ' || (s[*i] >= 9 && s[*i] <= 13)))
		(*i)++;
	if (s[*i])
		(*count)++;
	while (s[*i] && (s[*i] != ' ' && (s[*i] < 9 || s[*i] > 13)))
	{
		if (s[*i] == '\'' || s[*i] == '\"')
		{
			quote = s[*i];
			(*i)++;
			while (s[*i] && s[*i] != quote)
				(*i)++;
			if (s[*i] && s[*i] == quote)
				(*i)++;
		}
		else
			(*i)++;
	}
}
static int	countwords(char *s)
{
	int		i;
	char	quote;
	int		count;

	i = 0;
	count = 0;
	quote = 0;
	while (s[i])
		do_count_words_loop_sigma_best_ever_made(&i, s, quote, &count);
	return (count);
}

static void	quote_handle(int *len, char *s, int *i)
{
	char	quote;

	quote = s[*i];
	(*i)++;
	(*len)++;
	while (s[*i] && s[*i] != quote)
	{
		(*i)++;
		(*len)++;
	}
	if (s[*i] && s[*i] == quote)
		(*i)++;
	(*len)++;
}

static char	*wordalloc(char *s, char c, int *i)
{
	char	*word;
	char	*str;
	int		len;

	len = 0;
	while (s[*i] && (s[*i] == c || (s[*i] >= 9 && s[*i] <= 13)))
		(*i)++;
	str = s + (*i);
	while (s[*i] && (s[*i] != c || (s[*i] < 9 && s[*i] > 13)))
	{
		if (s[*i] && (s[*i] == '\'' || s[*i] == '\"'))
			quote_handle(&len, s, i);
		else
		{
			(*i)++;
			len++;
		}
	}
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	return (ft_strlcpy(word, str, len + 1), word);
}

char	**ft_arg_split(char *s, char c)
{
	char	**result;
	int		stringnum;
	int		i;
	int		index;

	index = 0;
	if (!s)
		return (NULL);
	i = 0;
	stringnum = countwords(s);
	result = malloc(sizeof(char *) * (stringnum + 1));
	if (!result)
		return (NULL);
	result[stringnum] = 0;
	while (i < stringnum)
	{
		result[i] = wordalloc(s, c, &index);
		if (!result[i])
			return (freetrix(result), NULL);
		i++;
	}
	return (result);
}

t_token	*expand_strs(t_token *tokens, t_mini *shell)
{
	int		i;
	int		j;
	int		k;
	int		amount;
	t_token	*new_tokens;
	char	**res;

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
	i = 0;
	k = 0;
	new_tokens = ft_calloc(amount + 1, sizeof(t_token));
	if (new_tokens == NULL)
		return (NULL);
	new_tokens[amount].type = T_NULL;
	new_tokens[amount].value = NULL;
	printf("amout = %d\n", amount);
	while (tokens[i].type != T_NULL)
	{
		if (!(tokens[i].type >= T_HERE_DOC && tokens[i].type <= T_APPEND_REDIR))
		{
			j = -1;
			res = ft_arg_split(tokens[i].value, ' ');
			if (res == NULL)
				amount++;
			while (res && res[++j])
			{
				new_tokens[k].value = ft_strdup(res[j]);
				if (tokens[i].type == T_PIPE)
					new_tokens[k].type = token_type(new_tokens[k].value, 1);
				else
					new_tokens[k].type = token_type(new_tokens[k].value, 0);
				k++;
			}
			i++;
			freetrix(res);
		}
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
