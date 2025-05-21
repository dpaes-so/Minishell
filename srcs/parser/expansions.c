/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniel <daniel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:58:38 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/05/21 02:26:43 by daniel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

int	new_tokens_amount(t_token *tokens, int i, int j)
{
	int		count;
	char	**res;
	int		amount;

	amount = 0;
	while (tokens[i].type != T_NULL)
	{
		count = 0;
		if (tokens[i].type >= T_HERE_DOC && tokens[i].type <= T_APPEND_REDIR)
			while (ft_strchr(" ><", tokens[i].value[count]) != NULL)
				count++;
		j = 0;
		res = ft_arg_split(tokens[i].value + count, ' ');
		if (res == NULL)
			amount++;
		while (res && res[j])
			j++;
		if (tokens[i].type >= T_HERE_DOC && tokens[i].type <= T_APPEND_REDIR
			&& j > 1)
			return (freetrix(res), -1);
		amount += j;
		i++;
		freetrix(res);
	}
	return (amount);
}

void	put_new_tokens(t_token *tokens, t_token *new_tokens, int *k, int *i)
{
	char	**res;
	int		j;

	j = -1;
	res = ft_arg_split(tokens[*i].value, ' ');
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
}

t_token	*create_new_tokens(t_token *tokens, int amount, int i, int k)
{
	t_token	*new_tokens;

	new_tokens = ft_calloc(amount + 1, sizeof(t_token));
	if (new_tokens == NULL)
		return (NULL);
	new_tokens[amount].type = T_NULL;
	new_tokens[amount].value = NULL;
	while (tokens[i].type != T_NULL)
	{
		if (!(tokens[i].type >= T_HERE_DOC && tokens[i].type <= T_APPEND_REDIR))
			put_new_tokens(tokens, new_tokens, &k, &i);
		else
		{
			new_tokens[k].value = ft_strdup(tokens[i].value);
			if (tokens[i].type == T_PIPE || (tokens[i].type >= T_HERE_DOC
					&& tokens[i].type <= T_APPEND_REDIR))
				new_tokens[k].type = token_type(new_tokens[k].value, 1);
			else
				new_tokens[k].type = token_type(new_tokens[k].value, 0);
			k++;
			i++;
		}
	}
	return (new_tokens);
}

t_token	*expand_strs(t_token *tokens, t_mini *shell)
{
	int		i;
	int		amount;
	t_token	*new_tokens;

	i = 0;
	amount = 0;
	while (tokens[i].type != T_NULL)
	{
		// printf("str = %s\n", tokens[i].value);
		if (tokens[i].type != T_PIPE)
		{
			if (tokens[i].type != T_HERE_DOC)
				dollar_expand(&tokens[i], shell);
		}
		i++;
	}
	amount = new_tokens_amount(tokens, 0, 0);
	if (amount < 0)
		return (printf("ambiguous redirect stoopid D:<\n"), NULL);
	new_tokens = create_new_tokens(tokens, amount, 0, 0);
	i = -1;
	while (new_tokens && new_tokens[++i].type != T_NULL)
		if (new_tokens[i].type != T_PIPE)
			remove_quotes(&new_tokens[i]);
	return (new_tokens);
}
