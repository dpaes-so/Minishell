/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_header.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:55:53 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/16 16:10:03 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_HEADER_H
# define MINI_HEADER_H

# include "libft/libft.h"
# include "limits.h"
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
	char				*copy;
	t_tokentype			type;
	bool				in_quotes;
}						t_token;

typedef struct s_cmd
{
	int					here_fd;
	int					amount;
	int					fdin;
	int					fdout;
	bool				pipe;
	char				*cmd;
	char				**args;
	t_token				*redir;
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
	int					pid1;
	int					cmd;
	int					infile_fd;
	int					outfile_fd;
	long				status;
	char				**path;
	int					pipefd[2];
}						t_pipe;
typedef struct s_mini
{
	int					wait_check;
	int					cmd_amount;
	char				*pwd;
	char				*input;
	int					save_fd;
	t_tree				*ast;
	t_env				*env;
	t_pipe				pipex;
}						t_mini;

//----------------------------BUILT-INS ! ! ! -----------------------------

int						do_redirect(t_cmd *cmds, t_mini *mini);
void					my_env_start(t_mini *mini, char **ev);
int						build_exit(t_mini *mini, t_cmd cmds);
int						build_echo(t_mini *mini, t_cmd cmds);
int						build_pwd(t_mini *mini, t_cmd cmds);
int						build_cd(t_mini *mini, t_cmd cmds);
int						build_env(t_mini *mini, t_cmd cmds);
int						build_unset(t_mini *mini, t_cmd cmds);
int						build_export(t_mini *mini, t_cmd cmds);
int						print_env_ex(t_mini *mini);
void					get_pwd(t_mini *mini);
void					pwd_update(t_mini *mini);
void					freetrix(char **matrix);
void					*add_export(t_mini *mini, char *arg);
char					*find_in_env(char *str, t_mini *shell);

//----------------------------EXECUTION ! ! ! -----------------------------

void					master_close(void);
void					exit_childprocess(t_mini *mini, int ecode);
char					**path_finder(char **envp);
void					execute(t_mini *mini, t_tree *ast, int f);
void					cmdexec(char *envp[], t_cmd cmds, t_mini *mini);
void					first_child(t_mini *mini, t_cmd cmds);
void					last_child(t_mini *mini, t_cmd cmds);
void					middle_child(t_mini *mini, t_cmd cmds);
void					solo_child(t_mini *mini, t_cmd cmds);
void					exit_childprocess_exec(t_mini *mini);
void					run_tree(t_mini *mini, t_tree *ast, int f);
void					wait_child(t_mini *mini);
void					cmd_exit(char *exec, t_mini *mini);
void					root_handler(int signal);
void					choose_signal(int s);
t_mini					*mem_save(t_mini *to_save);
int						here_doc(t_pipe pipex, t_cmd *cmds, int j,
							t_mini *mini);
char					**matrix_dup(t_mini *mini, char **ev);

//----------------------------PARSING ! ! ! -------------------------------

t_tree					*parser(char *input, t_mini *shell);
t_token					*split_tokens(char *input);
void					free_tokens(t_token *tokens);
int						count_tokens(char *input, t_token *result);
bool					word_alloc(char *input, int len, t_token *result,
							int i);
t_tokentype				token_type(char *value, int j);
bool					is_token(char **input, int *len);
bool					is_quote(char **input, int *len);
bool					skip_wspaces(char **input);
bool					check_next(char *input);
bool					check_redir(t_token tokens);
bool					error_syntax(t_token *tokens);
int						unclosed_quotes(t_token tokens);
int						count_nodes(t_token *tokens);
t_token					**array_creation(t_token *tokens);
void					init_array(t_token **array, t_token *tokens);
void					print_array(t_token **array);
void					create_tree(t_tree **tree_root, t_token **array,
							bool pipe, int *i);
void					init_tree_node(t_tree *tree_node, t_token *tokens);
void					tree_apply_infix(t_tree *root, int level, char *side);
void					count_cmds(t_tree *tree, t_mini *shell);
void					free_tree(t_tree *root);
void					free_array(t_token **array);

//-------------------------EXPANSIONS ! ! ! -------------------------------
bool					dollar_expand(t_token *token, t_mini *shell);
t_token					*expand_strs(t_token *tokens, t_mini *shell);
void					small_cpy(t_token *token, char *expand, int *j,
							int *amount);
void					handle_s_quote(t_token *token, char *expand, int *j);
void					handle_d_quote(t_token *token, t_mini *shell,
							char *expand, int *j);
void					handle_dollar(t_token *token, t_mini *shell,
							char *expand, int *j);
char					*status_expand(t_mini *shell);
char					*found_dollar(t_token *token, t_mini *shell, int *flag);
char					*find_env(t_token *token, t_mini *shell);

#endif