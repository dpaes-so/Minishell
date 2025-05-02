/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:25:00 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/05/02 17:24:45 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

t_tree	*tree_maker(t_token **array)
{
	t_tree	*root;
	int		i;

	i = 0;
	create_tree(&root, array, false, &i);
	return (root);
}

t_token	**create_array(t_token *tokens)
{
	t_token	**array;

	array = array_creation(tokens);
	init_array(array, tokens);
	return (array);
}
// print_array(array);

t_tree	*parser(char *input, t_mini *shell)
{
	t_token	*tokens;
	t_token	**array;
	t_tree	*tree;

	(void)shell;
	tokens = split_tokens(input);
	if (tokens == NULL)
		return (NULL);
	if (error_syntax(tokens) == false)
		return (free_tokens(tokens), NULL);
	expand_strs(tokens, shell);
	array = create_array(tokens);
	free_tokens(tokens);
	tree = tree_maker(array);
	count_cmds(tree, shell);
	free_array(array);
	return (tree);
}
