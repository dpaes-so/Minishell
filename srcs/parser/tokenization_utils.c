/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniel <daniel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 11:52:31 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/06/02 00:59:32 by daniel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

/// @brief Skips whitespaces of string
/// @param input The string
/// @return True if whitespaces were skipped
bool	skip_wspaces(char **input)
{
	if (**input && (**input == ' ' || (**input >= 9 && **input <= 13)))
	{
		while (**input && (**input == ' ' || (**input >= 9 && **input <= 13)))
			(*input)++;
		return (true);
	}
	return (false);
}

/// @brief Checks next token to see if its a pipe or a redirection
/// @param input Input from readline
/// @return True if there is no pipe or redirection
bool	check_next(char *input)
{
	if (*input && (*input == ' ' || (*input >= 9 && *input <= 13)))
	{
		while (*input && (*input == ' ' || (*input >= 9 && *input <= 13)))
		{
			input++;
		}
		if (ft_strchr("|<>", *input) == NULL)
			return (true);
	}
	return (false);
}

/// @brief Frees the array of tokens
/// @param tokens Array of tokens
void	free_tokens(t_token *tokens)
{
	int	i;

	if (tokens == NULL)
		return ;
	i = 0;
	while (tokens && tokens[i].type != T_NULL)
	{
		if (tokens[i].value)
			free(tokens[i].value);
		i++;
	}
	free(tokens);
}

/// @brief Identifies each token
/// @param value String of the token
/// @return The type of token
t_tokentype	token_type(char *value, int j)
{
	t_tokentype	type;
	int			i;

	i = 0;
	type = T_WORD;
	if (value == NULL)
		return (type);
	while (value[i] && (value[i] == ' ' || (value[i] >= 9 && value[i] <= 13)))
		i++;
	if (value[i] && value[i] == '|' && j == 1)
		type = T_PIPE;
	else if (value[i] && (value[i] == '>' || value[i] == '<') && j == 1)
	{
		if (value[i] == '>')
			type = T_OUT_REDIR;
		else if (value[i] == '<')
			type = T_IN_REDIR;
		i++;
		if (value[i] && (value[i] == '<'))
			type = T_HERE_DOC;
		if (value[i] && (value[i] == '>'))
			type = T_APPEND_REDIR;
	}
	return (type);
}

/// @brief Allocates memory for each element of the array of tokens
/// @param input Input from readline
/// @param len Length of the word
/// @param result The array of tokens
/// @param i Index to free allocated memory if error occurs
/// @return Will return true if everything goes well false if an error occurs
bool	word_alloc(char *input, int len, t_token *result, int i)
{
	result[i].value = ft_substr(input, 0, len);
	result[i].type = token_type(result[i].value, 1);
	result[i].in_quotes = false;
	result[i].ambiguous = false;
	if (result[i].value == NULL)
	{
		free_tokens(result);
		return (false);
	}
	i++;
	return (true);
}
