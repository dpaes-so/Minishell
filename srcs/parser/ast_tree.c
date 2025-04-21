/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tree.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:14:20 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/04/21 18:56:29 by dgarcez-         ###   ########.fr       */
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

int	count_elems(t_token *tokens)
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

t_token	**array_creation(t_token *tokens)
{
	int		i;
	int		index;
	int		len;
	int		amount;
	t_token	**array;

	i = 0;
	index = 0;
	amount = count_elems(tokens);
	array = ft_calloc(amount + 1, sizeof(t_token *));
	if (array == NULL)
		return (NULL);
	array[amount] = NULL;
	while (i < amount)
	{
		len = token_count(tokens, &index);
		array[i] = ft_calloc(len + 1, sizeof(t_token));
		if (array[i] == NULL)
			return (NULL);
		array[i][len].value = NULL;
		array[i][len].type = T_NULL;
		i++;
	}
	return (array);
}

void	init_array(t_token **array, t_token *tokens)
{
	int	i;
	int	j;
	int	k;

	i = -1;
	k = 0;
	while (array[++i])
	{
		j = 0;
		if (array[i][j].type != T_NULL && tokens[k].type == T_PIPE)
		{
			array[i][j].value = ft_strdup(tokens[k++].value);
			array[i]->type = T_PIPE;
			j++;
		}
		else
		{
			while (array[i][j].type != T_NULL && array[i][j].type != T_PIPE)
			{
				array[i][j].value = ft_strdup(tokens[k].value);
				array[i][j].type = tokens[k].type;
				k++;
				j++;
			}
		}
	}
}

void	free_array(t_token **array)
{
	int	i;
	int	j;

	i = 0;
	while (array[i])
	{
		j = 0;
		while (array[i][j].type != T_NULL)
		{
			free(array[i][j].value);
			j++;
		}
		free(array[i]);
		i++;
	}
	free(array);
}

void	init_redirs(t_tree *tree_node, t_token *tokens)
{
	int	i;
	int	j;
	int	count;

	j = 0;
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
	i = 0;
	while (tokens[i].type != T_NULL)
	{
		if (tokens[i].type >= T_HERE_DOC && tokens[i].type <= T_APPEND_REDIR)
		{
			tree_node->node.redirections[j].value = ft_strdup(tokens[i].value);
			tree_node->node.redirections[j].type = tokens[i].type;
			j++;
		}
		i++;
	}
}

void	init_tree_node(t_tree *tree_node, t_token *tokens)
{
	int		i;
	// char	*temp;

	i = 0;
	init_redirs(tree_node, tokens);
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
	else
	{
		tree_node->node.cmd = NULL;
		tree_node->node.args = NULL;
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

void	print_tree(t_tree *root, int level)
{
	int	i;

	i = 0;
	while (level != 0)
	{
		printf("\t");
		level--;
	}
	printf("cmd = %s\n", root->node.cmd);
	printf("args = %s\n", root->node.args);
	printf("pipe = %d\n", root->node.pipe);
	while (root->node.redirections[i].type != T_NULL)
	{
		printf("i = %d redir = %s\n", i, root->node.redirections[i].value);
		i++;
	}
}

void	tree_apply_infix(t_tree *root, int level)
{
	if (root->left != NULL)
		tree_apply_infix(root->left, level + 1);
	print_tree(root, level);
	if (root->right != NULL)
		tree_apply_infix(root->right, level + 1);
}

void	free_tree(t_tree *root)
{
	int	i;

	i = 0;
	if (!root)
		return ;
	if (root->node.args)
		free(root->node.args);
	if (root->node.cmd)
		free(root->node.cmd);
	while (root->node.redirections[i].value)
	{
		free(root->node.redirections[i].value);
		i++;
	}
	free(root->node.redirections);
	free_tree(root->left);
	free_tree(root->right);
	free(root);
}

void	create_array(t_token *tokens)
{
	int		i;
	int		j;
	t_token	**array;
	t_tree	*root;

	root = NULL;
	i = 0;
	j = 0;
	array = array_creation(tokens);
	init_array(array, tokens);
	while (array[i])
	{
		j = 0;
		while (array[i][j].type != T_NULL)
		{
			printf("i = %d j = %d value = %s$ type = %d\n", i, j,
					array[i][j].value, array[i][j].type);
			j++;
		}
		i++;
	}
	i = 0;
	create_tree(&root, array, false, &i);
	tree_apply_infix(root, 0);
	free_tree(root);
	free_array(array);
}
