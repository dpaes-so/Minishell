/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_header.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:55:53 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/03/12 18:36:46 by dgarcez-         ###   ########.fr       */
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
	char	*path;
	char	*input;
}			t_mini;

void	build_exit(t_mini *mini);
void	build_echo(t_mini *mini);

void	build_cd(t_mini *mini);
#endif