/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_tokens_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniel <daniel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 21:06:28 by daniel            #+#    #+#             */
/*   Updated: 2025/06/01 21:13:34 by daniel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

int	put_new_tokens(t_token *tokens, t_token *new_tokens, int *k, int *i)
{
	char	**res;
	int		j;
	int		flag;

	flag = 0;
	j = -1;
	res = ft_arg_split(tokens[*i].value, ' ', &flag);
	while (res && res[++j])
	{
		new_tokens[*k].value = ft_strdup(res[j]);
		if (tokens[*i].type == T_PIPE)
			new_tokens[*k].type = token_type(new_tokens[*k].value, 1);
		else
			new_tokens[*k].type = token_type(new_tokens[*k].value, 0);
		(*k)++;
	}
	(*i)++;
	freetrix(res);
	return (flag);
}

void	put_redir_tokens(t_token *tokens, t_token *new_tokens, int *k, int *i)
{
	new_tokens[*k].value = ft_strdup(tokens[*i].value);
	if (tokens[*i].type == T_PIPE || (tokens[*i].type >= T_HERE_DOC
			&& tokens[*i].type <= T_APPEND_REDIR))
		new_tokens[*k].type = token_type(new_tokens[*k].value, 1);
	else
		new_tokens[*k].type = token_type(new_tokens[*k].value, 0);
	(*k)++;
	(*i)++;
}

int	ambiguous_check(t_token *tokens, int i)
{
	int	count;

	count = 0;
	if (tokens[i].type >= T_HERE_DOC && tokens[i].type <= T_APPEND_REDIR)
		while (tokens[i].value[count] && ft_strchr(" ><",
				tokens[i].value[count]) != NULL)
			count++;
	if (tokens[i].value && !tokens[i].value[count]
		&& tokens[i].type >= T_HERE_DOC && tokens[i].type <= T_APPEND_REDIR)
		return (-1);
	return (count);
}
