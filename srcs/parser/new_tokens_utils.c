/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_tokens_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 21:06:28 by daniel            #+#    #+#             */
/*   Updated: 2025/06/02 16:14:10 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

void	put_null(t_token *tokens, t_token *new_tokens, int *k, int *i)
{
	new_tokens[*k].value = tokens[*i].value;
	new_tokens[*k].type = T_WORD;
	(*k)++;
}

int	put_new_tokens(t_token *tokens, t_token *new_tokens, int *k, int *i)
{
	char	**res;
	int		j;
	int		flag;

	flag = 0;
	j = -1;
	res = ft_arg_split(tokens[*i].value, ' ', &flag);
	if (flag == 1)
		return (flag);
	while (res && res[++j])
	{
		new_tokens[*k].value = ft_strdup(res[j]);
		if (tokens[*i].type == T_PIPE)
			new_tokens[*k].type = token_type(new_tokens[*k].value, 1);
		else
			new_tokens[*k].type = token_type(new_tokens[*k].value, 0);
		(*k)++;
	}
	if (res == NULL)
		put_null(tokens, new_tokens, k, i);
	(*i)++;
	freetrix(res);
	return (flag);
}

void	put_redir_tokens(t_token *tokens, t_token *new_tokens, int *k, int *i)
{
	new_tokens[*k].value = ft_strdup(tokens[*i].value);
	if (tokens[*i].type == T_PIPE || (tokens[*i].type >= T_HERE_DOC
			&& tokens[*i].type <= T_APPEND_REDIR))
		new_tokens[*k].type = tokens[*i].type;
	new_tokens[*k].ambiguous = tokens[*i].ambiguous;
	(*k)++;
	(*i)++;
}

int	ambiguous_check(t_token *tokens, int i)
{
	int	count;

	count = 0;
	if (tokens[i].type >= T_HERE_DOC && tokens[i].type <= T_APPEND_REDIR)
	{
		if (tokens[i].type == T_OUT_REDIR || tokens[i].type == T_IN_REDIR)
			count++;
		else if (tokens[i].type == T_APPEND_REDIR
			|| tokens[i].type == T_HERE_DOC)
			count += 2;
		while (tokens[i].value && tokens[i].value[count]
			&& ft_strchr(" \t\n\v\f\r", tokens[i].value[count]))
			count++;
	}
	if (tokens[i].value && !tokens[i].value[count]
		&& tokens[i].type >= T_HERE_DOC && tokens[i].type <= T_APPEND_REDIR)
		tokens[i].ambiguous = true;
	return (count);
}
