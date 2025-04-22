/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tree.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:14:20 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/04/23 00:01:26 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

void	init_redirs(t_tree *tree_node, t_token *tokens)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	j = 0;
	while (tokens[i].type != T_NULL)
	{
		if (tokens[i].type >= T_HERE_DOC && tokens[i].type <= T_APPEND_REDIR)
		{
			count = 0;
			while(ft_strchr(" ><", tokens[i].value[count]) != NULL)
				count++;
			tree_node->node.redirections[j].value = ft_strdup(tokens[i].value + count);
			tree_node->node.redirections[j].type = tokens[i].type;
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

void	make_args(t_tree *tree_node, t_token *tokens, int index)
{
	int	i;
	int	j;
	int	amount;

	amount = 0;
	j = 0;
	i = index;
	while (tokens[i].type != T_NULL)
	{
		if (tokens[i].type == T_WORD)
			amount++;
		i++;
	}
	tree_node->node.args = ft_calloc(amount + 1, sizeof(char *));
	if (tree_node->node.args == NULL)
		return ;
	tree_node->node.args[amount] = NULL;
	while(j < amount)
	{
		if (tokens[index].type == T_WORD)
			tree_node->node.args[j++] = ft_strdup(tokens[index].value);
		index++;
	}
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
	make_args(tree_node, tokens, i);
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

