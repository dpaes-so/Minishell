/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_header.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:55:53 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/03/24 19:37:56 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_HEADER_H
# define MINI_HEADER_H

# include "libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>

typedef struct s_mini
{
	char				*path;
	char				*input;
}						t_mini;

typedef struct s_cmd
{
	char				*cmd;
	char				*args;
	char				**redirections;
}						t_cmd;

typedef struct s_ast_node
{
	t_cmd				value;
	bool				pipe;

	struct s_ast_node	left;
	struct s_ast_node	rigth;

}						t_node;
void					build_exit(t_mini *mini);
void					build_echo(t_mini *mini);
void					build_cd(t_mini *mini);
#endif