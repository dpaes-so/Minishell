/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tree.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:14:20 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/04/22 20:03:14 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

void	init_redirs(t_tree *tree_node, t_token *tokens)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (tokens[i].type != T_NULL)
	{
		if (tokens[i].type >= T_HERE_DOC && tokens[i].type <= T_APPEND_REDIR)
		{
			tree_node->node.redirections[j].value = ft_strdup(tokens[i].value);
			tree_node->node.redirections[j].type = tokens[i].type;
			// skip_wspaces(&tree_node->node.redirections[j].value);
			j++;
		}
		i++;
	}
}

void	make_redirs(t_tree *tree_node, t_token *tokens)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (tokens[i].type != T_NULL)
	{
		if (tokens[i].type >= T_HERE_DOC && tokens[i].type <= T_APPEND_REDIR)
			count++;
		i++;
	}
	tree_node->node.redirections = ft_calloc(count + 1, sizeof(t_token));
	if (tree_node->node.redirections == NULL)
		return ;
	tree_node->node.redirections[count].value = NULL;
	tree_node->node.redirections[count].type = T_NULL;
	init_redirs(tree_node, tokens);
}

void	init_tree_node(t_tree *tree_node, t_token *tokens)
{
	int	i;

	i = 0;
	make_redirs(tree_node, tokens);
	while (tokens[i].type != T_NULL && tokens[i].type >= T_HERE_DOC
		&& tokens[i].type <= T_APPEND_REDIR)
		i++;
	if (tokens[i].type != T_NULL && tokens[i].type == T_WORD)
	{
		tree_node->node.cmd = ft_strdup(tokens[i].value);
		if (tree_node->node.cmd == NULL)
			return ;
		i++;
	}
	while (tokens[i].type != T_NULL)
	{
		if (tokens[i].type == T_WORD)
			tree_node->node.args = ft_strjoin(tree_node->node.args,
												tokens[i].value);
		i++;
	}
}

t_tree	*tree_create_node(t_token *tokens, int pipe)
{
	t_tree	*tree_node;

	tree_node = malloc(sizeof(t_tree));
	if (tree_node == NULL)
		return (NULL);
	tree_node->left = NULL;
	tree_node->right = NULL;
	tree_node->node.args = NULL;
	tree_node->node.cmd = NULL;
	tree_node->node.redirections = NULL;
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
	if (array[*i])
	{
		while (array[*i] != NULL && array[*i]->type != T_PIPE && pipe == false)
			(*i)++;
		if (array[*i] != NULL && array[*i]->type == T_PIPE && pipe == false)
		{
			*tree_root = tree_create_node(array[*i], 1);
			*i -= 1;
			create_tree(&((*tree_root)->right), array, true, i);
			*i += 2;
			create_tree(&((*tree_root)->left), array, false, i);
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

