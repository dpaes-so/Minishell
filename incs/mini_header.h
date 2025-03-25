/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_header.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:55:53 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/03/25 16:46:22 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_HEADER_H
# define MINI_HEADER_H

# include "libft/libft.h"
# include "stdbool.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_cmd
{
	char				*cmd;
	char				*args;
	char				**redirections;
}						t_cmd;

typedef struct s_ast_tree
{
	t_cmd				value;
	bool				pipe;
	struct s_ast_node	*left;
	struct s_ast_node	*right;

}						t_tree;

typedef struct s_mini
{
	char				*path;
	char				*input;
	t_tree				*ast;
}						t_mini;
void					build_exit(t_mini *mini);
void					build_echo(t_mini *mini);
void					build_cd(t_mini *mini);
void					get_pwd(t_mini *mini);
void					freetrix(char **matrix);
#endif