/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_header.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:55:53 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/04/09 18:26:08 by dpaes-so         ###   ########.fr       */
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

typedef enum TokenType
{
	T_WORD,
	T_PIPE,
	T_HERE_DOC,
	T_IN_REDIR,
	T_OUT_REDIR,
	T_APPEND_REDIR,
}						t_tokentype;

typedef struct s_token
{
	char				*value;
	t_tokentype			type;
}						t_token;

typedef struct s_cmd
{
	bool				pipe;
	char				*cmd;
	char				*args;
	t_token				*redirections;
}						t_cmd;

typedef struct s_ast_tree
{
	t_cmd				node;
	struct s_ast_node	*left;
	struct s_ast_node	*right;

}						t_tree;

typedef struct s_env
{
	char				**my_env;
	char				*home;
}						t_env;

typedef struct s_mini
{
	char				*pwd;
	char				*input;
	t_tree				*ast;
	t_env				*env;
}						t_mini;

void					my_env_start(t_mini *mini, char **ev);
void					build_exit(t_mini *mini);
void					build_echo(t_mini *mini);
int						build_pwd(t_mini *mini);
int						build_cd(t_mini *mini);
int						build_env(t_mini *mini);
void					get_pwd(t_mini *mini);
void					freetrix(char **matrix);
#endif