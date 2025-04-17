/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_header.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:55:53 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/04/17 19:08:34 by dgarcez-         ###   ########.fr       */
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
	T_NULL,
}						t_tokentype;

typedef struct s_token
{
	int					index;
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

/// @brief tree lil bro
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
	int					error_code;
}						t_env;

typedef struct s_mini
{
	char				*pwd;
	char				*input;
	t_tree				*ast;
	t_env				*env;
}						t_mini;

//----------------------------BUILT-INS ! ! ! -----------------------------------

void					my_env_start(t_mini *mini, char **ev);
void					build_exit(t_mini *mini);
void					build_echo(t_mini *mini);
int						build_pwd(t_mini *mini);
int						build_cd(t_mini *mini);
int						build_env(t_mini *mini);
int						build_unset(t_mini *mini);
int						build_export(t_mini *mini);
void					get_pwd(t_mini *mini);
void					freetrix(char **matrix);

//----------------------------PARSING ! ! ! -------------------------------------

void					parser(char	*input);
t_token					*split_tokens(char *input);
void					free_tokens(t_token *tokens);
int						count_tokens(char *input, t_token *result);
bool					word_alloc(char *input, int len, t_token *result, int i);
t_tokentype				token_type(char *value);
bool					is_token(char **input, int *len, int *flag);
bool					is_quote(char **input, int *len);
bool					skip_wspaces(char **input);
bool					check_next(char *input);
bool					check_redir(t_token tokens);
bool					error_syntax(t_token *tokens);
int						count_nodes(t_token *tokens);
void					create_tree(t_token *tokens);

#endif