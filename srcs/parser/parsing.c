/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniel <daniel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:25:00 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/06/01 01:15:06 by daniel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

t_tree	*tree_maker(t_token **array, t_mini *shell)
{
	t_tree	*root;
	int		i;

	root = NULL;
	i = 0;
	if (create_tree(&root, array, false, &i) == 1)
	{
		free_tree(root);
		free_array(array);
		fmalloc(shell, "tree_maker", 2);
	}
	return (root);
}

t_token	**create_array(t_token *tokens, t_mini *shell)
{
	t_token	**array;

	array = NULL;
	array = array_creation(tokens, shell);
	if (shell->f_malloc == 1)
	{
		free_tokens(tokens);
		fmalloc(shell, "array_creation", 2);
	}
	init_array(array, tokens);
	return (array);
}
// print_array(array);

t_tree	*parser(char *input, t_mini *shell)
{
	t_token	*tokens;
	t_token	*new_tokens;
	t_token	**array;
	t_tree	*tree;

	tokens = NULL;
	new_tokens = NULL;
	array = NULL;
	tree = NULL;
	tokens = split_tokens(input, shell);
	if (tokens == NULL)
		return (freetrix(shell->pipex.path), NULL);
	if (error_syntax(shell, tokens) == false)
		return (free_tokens(tokens), freetrix(shell->pipex.path), NULL);
	new_tokens = expand_strs(tokens, shell);
	if (new_tokens == NULL)
		return (free_tokens(tokens), freetrix(shell->pipex.path), NULL);
	free_tokens(tokens);
	array = create_array(new_tokens, shell);
	free_tokens(new_tokens);
	tree = tree_maker(array, shell);
	if (tree == NULL)
		return (freetrix(shell->pipex.path), free_array(array), NULL);
	count_cmds(tree, shell);
	return (free_array(array), tree);
}
