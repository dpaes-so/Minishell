/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:25:00 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/04/23 14:23:03 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

void	tree_maker(t_token **array)
{
	t_tree	*root;
	int		i;

	i = 0;
	create_tree(&root, array, false, &i);
	tree_apply_infix(root, 0, "root");
	free_tree(root);
}

t_token	**create_array(t_token *tokens)
{
	t_token	**array;

	array = array_creation(tokens);
	init_array(array, tokens);
	return (array);
}
	// print_array(array);

void	parser(char *input)
{
	t_token	*tokens;
	t_token	**array;

	tokens = split_tokens(input);
	if (tokens == NULL)
		return ;
	if (error_syntax(tokens) == false)
		return (free_tokens(tokens));
	array = create_array(tokens);
	free_tokens(tokens);
	tree_maker(array);
	free_array(array);
}
