/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tree.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:14:20 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/05/20 17:52:36 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

t_tree	*tree_create_node(t_token *tokens, int pipe)
{
	t_tree	*tree_node;

	tree_node = malloc(sizeof(t_tree));
	if (tree_node == NULL)
		return (NULL);
	tree_node->left = NULL;
	tree_node->right = NULL;
	tree_node->node.amount = 0;
	tree_node->node.args = NULL;
	tree_node->node.cmd = NULL;
	tree_node->node.fdin = -1;
	tree_node->node.fdout = -1;
	tree_node->node.redir = NULL;
	if (pipe == 1)
		tree_node->node.pipe = true;
	else
	{
		tree_node->node.pipe = false;
		init_tree_node(tree_node, tokens);
	}
	return (tree_node);
}

void	create_tree(t_tree **tree_root, t_token **array, bool pipe, int *i)
{
	if (*i < 0)
		return ;
	if (array && array[*i])
	{
		while (array[*i] != NULL && array[*i]->type != T_PIPE && pipe == false)
			(*i)++;
		if (array[*i] != NULL && array[*i]->type == T_PIPE && pipe == false)
		{
			*tree_root = tree_create_node(array[*i], 1);
			*i -= 1;
			create_tree(&((*tree_root)->left), array, true, i);
			*i += 2;
			create_tree(&((*tree_root)->right), array, false, i);
			return ;
		}
		else
		{
			if (pipe == false)
				*i -= 1;
			*tree_root = tree_create_node(array[*i], 0);
			return ;
		}
	}
}
