/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:39:31 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/04/23 14:23:57 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

void	print_tabs(int level)
{
	while (level != 0)
	{
		printf("\t");
		level--;
	}
}

void	print_tree(t_tree *root, int level, char *side)
{
	int	i;

	i = 0;
	print_tabs(level);
	printf("%s side: \n", side);
	if (root->node.cmd)
	{
		print_tabs(level);
		printf("cmd = %s\n", root->node.cmd);
	}
	if (root->node.args)
	{
		while (root->node.args[i])
		{
			print_tabs(level);
			printf("args = %s\n", root->node.args[i]);
			i++;
		}
		i = 0;
	}
	if (root->node.pipe == true)
	{
		print_tabs(level);
		printf("|\n");
	}
	while (root->node.redirections != NULL
		&& root->node.redirections[i].type != T_NULL)
	{
		print_tabs(level);
		printf("redir = %s type = %u\n", root->node.redirections[i].value,
			root->node.redirections[i].type);
		i++;
	}
}

void	tree_apply_infix(t_tree *root, int level, char *side)
{
	if (root->left != NULL)
		tree_apply_infix(root->right, level + 1, "right");
	print_tree(root, level, side);
	if (root->right != NULL)
		tree_apply_infix(root->left, level + 1, "left");
}

void	free_tree(t_tree *root)
{
	int	i;

	i = 0;
	if (!root)
		return ;
	while (root->node.args && root->node.args[i])
	{
		free(root->node.args[i]);
		i++;
	}
	if (root->node.args)
		free(root->node.args);
	if (root->node.cmd)
		free(root->node.cmd);
	i = 0;
	while (root->node.redirections != NULL && root->node.redirections[i].value)
	{
		free(root->node.redirections[i].value);
		i++;
	}
	free(root->node.redirections);
	free_tree(root->left);
	free_tree(root->right);
	free(root);
}

void	print_array(t_token **array)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
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
}
