/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:06:39 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/04/18 19:45:34 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

/// @brief Handles quotes by skipping the string until the next quote
/// @param input Input from readline
/// @param len Length of the string
/// @return True if quotes were handled
bool	is_quote(char **input, int *len)
{
	char	quote;

	if (**input == '\'' || **input == '\"')
	{
		quote = **input;
		if (**input && **input == quote)
		{
			(*input)++;
			(*len)++;
		}
		while (**input && (**input != quote))
		{
			(*input)++;
			(*len)++;
		}
		if (**input && **input == quote)
		{
			(*input)++;
			(*len)++;
		}
		return (true);
	}
	return (false);
}

/// @brief Goes through the input and verifies if it is a word
/// @param input Input from readline
/// @param len Length of the token string
/// @param flag When flag is 0 and the next token isnt a pipe nor redirection
///	will increase len by one to add a space
/// @return True when its just a word false if it is a pipe or redirection
bool	is_word(char **input, int *len, int flag)
{
	if (**input && ft_strchr("|<>", **input) == NULL)
	{
		while (**input && ft_strchr(" \t\n\v\f\r|<>", **input) == NULL)
		{
			if (is_quote(input, len) == false)
			{
				(*input)++;
				(*len)++;
			}
		}
		if (**input && flag == 0 && check_next(*input))
		{
			(*len)++;
			(*input)++;
		}
		return (true);
	}
	return (false);
}

/// @brief Checks if input from readline is a token or not
/// @param input Input from readline
/// @param len Length of the token string
/// @param flag Flag is 1 whenever there is a space at the end of the word
/// @return Returns true when it is a token false if not
bool	is_token(char **input, int *len, int *flag)
{
	if (**input && ft_strchr("|<>", **input) != NULL)
	{
		if (**input == '|')
		{
			(*input)++;
			(*len)++;
			return (true);
		}
		while (**input && (**input == '>' || **input == '<'))
		{
			(*len)++;
			(*input)++;
		}
		while (**input && (**input == ' ' || (**input >= 9 && **input <= 13)))
		{
			(*len)++;
			(*input)++;
		}
		is_word(input, len, 1);
		if (**input && (**input == ' ' || (**input >= 9 && **input <= 13)))
			*flag = 1;
		return (true);
	}
	return (false);
}

/// @brief Counts how many tokens the input has
/// and if the array of result has been allocated
///	each iteration will go to the word_alloc function.
/// @param input input from readline
/// @param result the array of tokens
/// @return amount of tokens
int	count_tokens(char *input, t_token *result)
{
	int	len;
	int	count;
	int	flag;

	flag = 0;
	count = 0;
	if (!input)
		return (0);
	while (*input)
	{
		skip_wspaces(&input);
		len = 0;
		if (*input && flag == 1 && *input != '|')
			len++;
		flag = 0;
		if (is_word(&input, &len, 0) == true)
			count++;
		else if (is_token(&input, &len, &flag) == true)
			count++;
		if (len && result && word_alloc(input - len, len, result, count
				- 1) == false)
			return (0);
	}
	return (count);
}

/// @brief Splits input into an array of tokens, each token with a value
/// and the type
/// @param input input from readline
/// @return an array of a struct t_token
t_token	*split_tokens(char *input)
{
	t_token	*token;
	int		amount;

	amount = count_tokens(input, NULL);
	token = ft_calloc(amount + 1, sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token[amount].value = NULL;
	token[amount].type = T_NULL;
	if (count_tokens(input, token) == 0)
		return (free(token), NULL);
	return (token);
}
