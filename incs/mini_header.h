/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_header.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:55:53 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/04/23 17:15:18 by dgarcez-         ###   ########.fr       */
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
	char				*value;
	t_tokentype			type;
}						t_token;

typedef struct s_cmd
{
	bool				pipe;
	char				*cmd;
	char				**args;
	t_token				*redirections;
}						t_cmd;

/// @brief tree lil bro
typedef struct s_ast_tree
{
	t_cmd				node;
	struct s_ast_tree	*left;
	struct s_ast_tree	*right;
}						t_tree;

typedef struct s_env
{
	char				**my_env;
	char				*home;
	int					error_code;
}						t_env;

typedef struct s_pipe
{
	int		pid1;
	int		infile_fd;
	int		outfile_fd;
	int 	status;
	int		pipefd[2];
}			t_pipe;
typedef struct s_mini
{

	char				*pwd;
	char				*input;
	t_tree				*ast;
	t_env				*env;
	t_pipe 				pipex;
}						t_mini;

//----------------------------BUILT-INS ! ! ! -----------------------------

void					my_env_start(t_mini *mini, char **ev);
void					build_exit(t_mini *mini,t_cmd cmds);
void					build_echo(t_mini *mini);
int						build_pwd(t_mini *mini);
int						build_cd(t_mini *mini,t_cmd cmds);
int						build_env(t_mini *mini);
int						build_unset(t_mini *mini,t_cmd cmds);
int						build_export(t_mini *mini,t_cmd cmds);
void					get_pwd(t_mini *mini);
void					freetrix(char **matrix);

//----------------------------PARSING ! ! ! -------------------------------

t_tree					*parser(char *input);
t_token					*split_tokens(char *input);
void					free_tokens(t_token *tokens);
int						count_tokens(char *input, t_token *result);
bool					word_alloc(char *input, int len, t_token *result,
							int i);
t_tokentype				token_type(char *value);
bool					is_token(char **input, int *len);
bool					is_quote(char **input, int *len);
bool					skip_wspaces(char **input);
bool					check_next(char *input);
bool					check_redir(t_token tokens);
bool					error_syntax(t_token *tokens);
int						count_nodes(t_token *tokens);
t_token					**array_creation(t_token *tokens);
void					init_array(t_token **array, t_token *tokens);
void					print_array(t_token **array);
void					create_tree(t_tree **tree_root, t_token **array,
							bool pipe, int *i);
void					init_tree_node(t_tree *tree_node, t_token *tokens);
void					tree_apply_infix(t_tree *root, int level, char *side);
void					free_tree(t_tree *root);
void					free_array(t_token **array);

//-------------------------EXPANSIONS ! ! ! -------------------------------
void	expand_strs(t_token *tokens);

#endif