/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 11:52:31 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/04/16 19:12:29 by dgarcez-         ###   ########.fr       */
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
