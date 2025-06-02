/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_creation_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 01:25:13 by root              #+#    #+#             */
/*   Updated: 2025/06/02 16:03:29 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

void	init_redirs(t_tree *tree_node, t_token *tok)
{
	int	i;
	int	j;
	int	count;

	i = -1;
	j = 0;
	while (tok[++i].type != T_NULL)
	{
		if (tok[i].value && tok[i].type >= T_HERE_DOC
			&& tok[i].type <= T_APPEND_REDIR)
		{
			count = 0;
			if (tok[i].type == T_OUT_REDIR || tok[i].type == T_IN_REDIR)
				count++;
			else if (tok[i].type == T_APPEND_REDIR || tok[i].type == T_HERE_DOC)
				count += 2;
			while (tok[i].value && tok[i].value[count]
				&& ft_strchr(" \t\n\v\f\r", tok[i].value[count]))
				count++;
			tree_node->node.redir[j].value = ft_strdup(tok[i].value + count);
			tree_node->node.redir[j].type = tok[i].type;
			tree_node->node.redir[j].in_quotes = tok[i].in_quotes;
			tree_node->node.redir[j++].ambiguous = tok[i].ambiguous;
		}
	}
}

int	make_redirs(t_tree *tree_node, t_token *tokens)
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
	tree_node->node.redir = ft_calloc(count + 1, sizeof(t_token));
	if (tree_node->node.redir == NULL)
		return (1);
	tree_node->node.redir[count].value = NULL;
	tree_node->node.redir[count].type = T_NULL;
	init_redirs(tree_node, tokens);
	return (0);
}

int	make_args(t_tree *tree_node, t_token *tokens, int index, int j)
{
	int	i;
	int	amount;

	amount = 0;
	i = -1;
	while (tokens[++i].type != T_NULL)
		if (tokens[i].value != NULL && tokens[i].type == T_WORD)
			amount++;
	tree_node->node.args = ft_calloc(amount + 1, sizeof(char *));
	if (tree_node->node.args == NULL)
		return (1);
	tree_node->node.args[amount] = NULL;
	while (j < amount)
	{
		if (tokens[index].value != NULL && tokens[index].type == T_WORD)
		{
			tree_node->node.args[j++] = ft_strdup(tokens[index].value);
			if (tree_node->node.args[j - 1] == NULL)
				return (1);
		}
		index++;
	}
	tree_node->node.amount = j;
	return (0);
}

int	init_tree_node(t_tree *tree_node, t_token *tokens)
{
	int	i;

	i = 0;
	if (make_redirs(tree_node, tokens) == 1)
		return (1);
	while (tokens[i].type != T_NULL && tokens[i].type >= T_HERE_DOC
		&& tokens[i].type <= T_APPEND_REDIR)
		i++;
	while (tokens[i].type != T_NULL)
	{
		if (tokens[i].value != NULL && tokens[i].type != T_NULL
			&& tokens[i].type == T_WORD)
		{
			tree_node->node.cmd = ft_strdup(tokens[i].value);
			if (tree_node->node.cmd == NULL)
				return (1);
			break ;
		}
		i++;
	}
	if (make_args(tree_node, tokens, 0, 0) == 1)
		return (1);
	return (0);
}

void	count_cmds(t_tree *tree, t_mini *shell)
{
	if (!tree || tree == NULL)
		return ;
	if (tree->node.pipe == false)
		shell->cmd_amount++;
	count_cmds(tree->left, shell);
	count_cmds(tree->right, shell);
}
