/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:19:28 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/04/14 16:14:12 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/// @brief Counts how many quotes a token has.
/// @param tokens The token it will inspect.
/// @return amount of quotes found if the amount of quotes is odd.
int	unclosed_quotes(t_token tokens)
{
	int		i;
	int		quote;
	char	cquote;

	i = 0;
	quote = 0;
	while (tokens.value[i])
	{
		if (tokens.value[i] == '\"' || tokens.value[i] == '\'')
		{
			quote++;
			cquote = tokens.value[i];
			i++;
			while (tokens.value[i] && tokens.value[i] != cquote)
				i++;
			if (tokens.value[i] == cquote)
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

/// @brief Checks for every string of a token to see if an & exists out of quotes
/// @param tokens The token it will inspect
/// @return True if no & was found out of quotes
bool	check_and(t_token tokens)
{
	int		i;
	char	quote;

	quote = 0;
	i = 0;
	while (tokens.value[i] != '\0')
	{
		if (tokens.value[i] == '&')
			return (false);
		if (tokens.value[i] == '\"' || tokens.value[i] == '\'')
		{
			quote = tokens.value[i];
			i++;
			while (tokens.value[i] && tokens.value[i] != quote)
				i++;
			if (tokens.value[i] == quote)
				i++;
		}
		else
			i++;
	}
	return (true);
}
/// @brief Checks the syntax for redirection cases
/// @param tokens Array of tokens
/// @return True if all checks passed
bool	check_redir(t_token tokens)
{
	int		i;
	char	redir;

	i = 0;
	if (tokens.type != T_PIPE && tokens.type != T_WORD)
	{
		while (tokens.value[i] == ' ' || (tokens.value[i] >= 9
				&& tokens.value[i] <= 13))
			i++;
		redir = tokens.value[i];
		i++;
		if (tokens.value[i] && ft_strchr("<>", tokens.value[i]) != NULL
			&& tokens.value[i] != redir)
			return (false);
		else if (tokens.value[i] && ft_strchr("<>", tokens.value[i]) != NULL)
			i++;
		if (tokens.value[i] && ft_strchr("<>", tokens.value[i]) != NULL)
			return (false);
		while (tokens.value[i] == ' ' || (tokens.value[i] >= 9
				&& tokens.value[i] <= 13))
			i++;
		if (tokens.value[i] == '\0')
			return (false);
	}
	return (true);
}

/// @brief Checks the syntax for pipe cases
/// @param tokens Array of tokens
/// @param amount Amount of tokens
/// @return True if all checks passed
bool	pipe_syntax(t_token *tokens, int amount)
{
	t_token	temp;
	int		i;

	i = 0;
	if (tokens[0].type == T_PIPE)
		return (false);
	if (amount > 1)
	{
		temp = tokens[i + 1];
		while (i < amount)
		{
			if (tokens[i + 1].value)
				temp = tokens[i + 1];
			else
				break ;
			if (tokens[i].type == T_PIPE && temp.type == T_PIPE)
				return (false);
			i++;
		}
		if (tokens[i].type == T_PIPE && tokens[i + 1].value == NULL)
			return (false);
	}
	return (true);
}

/// @brief Goes through every syntax error check
/// @param tokens array of tokens
/// @param amount amount of tokens
/// @return True if all checks passed
bool	error_syntax(t_token *tokens, int amount)
{
	int	i;

	i = 0;
	if (pipe_syntax(tokens, amount) == false)
	{
		printf("error syntax lil bro\n");
		return (false);
	}
	while (i < amount)
	{
		printf("token = %s$ type = %u\n", tokens[i].value, tokens[i].type);
		if (check_redir(tokens[i]) == false || check_and(tokens[i]) == false)
			printf("syntax error noob\n");
		if (unclosed_quotes(tokens[i]) % 2 != 0)
			printf("quotes aint closed dumbass\n");
		i++;
	}
	return (true);
}
