/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tree.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:14:20 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/04/17 20:40:59 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

// void	put_redir(t_token *redirs, t_token *tokens, int pos)
// {
// 	int	i;
// 	int	j;

// 	j = 0;
// 	i = 0;
// 	while(i < pos)
// 	{
// 		if (tokens[i].type >= T_HERE_DOC || tokens[i].type <= T_APPEND_REDIR)
// 		{
// 			redirs[j].type = tokens[i].type;
// 			redirs[j].value = ft_strdup(tokens[i].value);
// 			j++;
// 		}
// 		i++;
// 	}
// }

// t_token	*get_redirs(t_token *tokens, int pos)
// {
// 	int		i;
// 	int		count;
// 	t_token	*redirs;

// 	count = 0;
// 	i = 0;
// 	while (i <= pos)
// 	{
// 		if (tokens[i].type >= T_HERE_DOC || tokens[i].type <= T_APPEND_REDIR)
// 			count++;
// 		i++;
// 	}
// 	if (count == 0)
// 		return (NULL);
// 	redirs = ft_calloc(count + 1, sizeof(t_token));
// 	if (redirs == NULL)
// 		return (NULL);
// 	redirs = put_redir(redirs, tokens, pos);
// 	return (redirs);
// }

// t_tree	*create_node(t_token *tokens, int pos)
// {
// 	t_tree	*ast;

// 	ast = malloc(sizeof(t_tree));
// 	if (ast == NULL)
// 		return (NULL);
// 	ast->left = NULL;
// 	ast->right = NULL;
// 	if (pos == 0)
// 		ast->node.cmd = token.value;
// 	else
// 		ast->node.cmd = NULL;
// 	ast->node.redirections = get_redirs(tokens, pos);
// 		if (token.type == T_PIPE)
// 			ast->node.pipe = true;
// 	return (node);
// }

int	count_nodes(t_token *tokens)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (tokens[i].type != T_NULL)
	{
		if (tokens[i].type != T_PIPE)
			count++;
		while (tokens[i].type != T_NULL && tokens[i].type != T_PIPE)
			i++;
		if (tokens[i].type != T_NULL && tokens[i].type == T_PIPE)
		{
			count++;
			i++;
		}
	}
	return (count);
}

int	token_count(t_token *tokens, int *i)
{
	int	amount;

	amount = 0;
	if (tokens[*i].type == T_PIPE)
	{
		(*i)++;
		return (1);
	}
	else
	{
		while (tokens[*i].type != T_NULL && tokens[*i].type != T_PIPE)
		{
			(*i)++;
			amount++;
		}
	}
	return (amount);
}

t_token	**node_creation(t_token *tokens)
{
	int		i;
	int		index;
	int		len;
	int		amount;
	t_token	**nodes;

	i = 0;
	index = 0;
	amount = count_nodes(tokens);
	nodes = ft_calloc(amount + 1, sizeof(t_token *));
	if (nodes == NULL)
		return (NULL);
	nodes[amount] = NULL;
	while (i < amount)
	{
		len = token_count(tokens, &index);
		nodes[i] = ft_calloc(len + 1, sizeof(t_token));
		if (nodes[i] == NULL)
			return (NULL);
		nodes[i][len].value = NULL;
		nodes[i][len].type = T_NULL;
		i++;
	}
	return (nodes);
}

void	init_nodes(t_token **nodes, t_token *tokens)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	k = 0;
	while (nodes[i])
	{
		j = 0;
		while (nodes[i][j].type != T_NULL)
		{
			nodes[i][j].value = ft_strdup(tokens[k].value);
			k++;
			j++;
		}
		i++;
	}
}

void	create_tree(t_token *tokens)
{
	int i;
	int j;

	i = 0;
	j = 0;
	t_token **nodes;

	nodes = node_creation(tokens);
	init_nodes(nodes, tokens);
	while (nodes[i])
	{
		while (nodes[i][j].type != T_NULL)
		{
			printf("%s\n", nodes[i][j].value);
			j++;
		}
		i++;
	}
}