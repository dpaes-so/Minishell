/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   final_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniel <daniel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 19:35:39 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/06/02 01:24:20 by daniel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

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

static char	*wordalloc(char *s, char c, int *i, int *flag)
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
		return (*flag = 1, NULL);
	return (ft_strlcpy(word, str, len + 1), word);
}

char	**ft_arg_split(char *s, char c, int *flag)
{
	char	**result;
	int		stringnum;
	int		i;
	int		index;

	index = 0;
	if (!s || s[0] == '\0')
		return (NULL);
	i = 0;
	stringnum = countwords(s);
	result = malloc(sizeof(char *) * (stringnum + 1));
	if (!result)
		return (*flag = 1, NULL);
	result[stringnum] = 0;
	while (i < stringnum)
	{
		result[i] = wordalloc(s, c, &index, flag);
		if (!result[i])
			return (freetrix(result), NULL);
		i++;
	}
	return (result);
}
