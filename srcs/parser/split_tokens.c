/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:06:39 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/04/16 16:58:10 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

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
	int	check;

	check = 0;
	if (**input && ft_strchr("|<>", **input) == NULL)
	{
		while (**input && ft_strchr(" |<>", **input) == NULL)
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

/// @brief Identifies each token
/// @param value String of the token
/// @return The type of token
t_tokentype	token_type(char *value)
{
	t_tokentype	type;
	int			i;

	i = 0;
	type = T_WORD;
	while (value[i] && (value[i] == ' ' || (value[i] >= 9 && value[i] <= 13)))
		i++;
	if (value[i] && value[i] == '|')
		type = T_PIPE;
	else if (value[i] && (value[i] == '>' || value[i] == '<'))
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

/// @brief Allocates memory for each element of the array result
/// @param input Input from readline
/// @param len Length of the word
/// @param result The array of tokens
/// @param i Index to free allocated memory if error occurs
/// @return Will return true if everything goes well false if an error occurs
bool	word_alloc(char *input, int len, t_token *result, int i)
{
	result[i].value = ft_substr(input, 0, len);
	result[i].type = token_type(result[i].value);
	if (result[i].value == NULL)
	{
		i = 0;
		while (result[i].value)
		{
			free(result[i].value);
			i++;
		}
		free(result);
		return (false);
	}
	i++;
	return (true);
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
	token[amount].type = T_WORD;
	if (count_tokens(input, token) == 0)
		return (free(token), NULL);
	return (token);
}

void	free_tokens(t_token *tokens, int amount)
{
	int	i;
	
	i = 0;
	while(i < amount)
	{
		if (tokens[i].value)
			free(tokens[i].value);
		i++;
	}
	free (tokens);
}

int	main(void)
{
	char	*input;
	t_token	*split;
	int		amount;
	int		i;

	while (1)
	{
		i = 0;
		input = readline("minishell > ");
		add_history(input);
		printf("str = %s\n", input);
		split = split_tokens(input);
		amount = count_tokens(input, NULL);
		printf("amount lil bro = %d\n", amount);
		if (amount == 0)
			continue;
		while (i < amount)
		{
			printf("token = %s$ type = %u\n", split[i].value, split[i].type);
			if (check_redir(split[i]) == false)
				printf("syntax error noob\n");
			i++;
		}
		error_syntax(split, amount);
		free_tokens(split, amount);
		if (strcmp(input, "exit") == 0)
			exit(0);
		free(input);
	}
}
