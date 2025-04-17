/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:25:00 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/04/17 19:08:20 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

void	parser(char *input)
{
	int i;
	t_token *tokens;

	i = 0;
	tokens = split_tokens(input);
	if (tokens == NULL)
		return ;
	while (tokens[i].type != T_NULL)
	{
		printf("token = %s$ type = %u\n", tokens[i].value, tokens[i].type);
		if (check_redir(tokens[i]) == false)
			printf("syntax error noob\n");
		i++;
	}
	error_syntax(tokens);
	create_tree(tokens);
	free_tokens(tokens);
}
