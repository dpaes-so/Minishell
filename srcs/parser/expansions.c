/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniel <daniel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:58:38 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/06/01 02:09:01 by daniel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

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

int	new_tokens_amount(t_token *tokens, int i, int j, int *flag)
{
	int		count;
	char	**res;
	int		amount;

	amount = 0;
	while (tokens[i].type != T_NULL)
	{
		j = 0;
		count = ambiguous_check(tokens, i);
		if (count == -1)
			return (-1);
		res = ft_arg_split(tokens[i].value + count, ' ', flag);
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

t_token	*create_new_tokens(t_token *tokens, int amount, int i, t_mini *shell)
{
	t_token	*new_tokens;
	int		k;

	k = 0;
	new_tokens = ft_calloc(amount + 1, sizeof(t_token));
	if (new_tokens == NULL)
	{
		shell->f_malloc = 1;
		return (NULL);
	}
	new_tokens[amount].type = T_NULL;
	new_tokens[amount].value = NULL;
	while (tokens[i].type != T_NULL)
	{
		if (!(tokens[i].type >= T_HERE_DOC && tokens[i].type <= T_APPEND_REDIR))
		{
			if (put_new_tokens(tokens, new_tokens, &k, &i) == 1)
			{
				shell->f_malloc = 1;
				return (NULL);
			}
		}
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

	i = -1;
	amount = 0;
	while (tokens[++i].type != T_NULL)
	{
		if (tokens[i].type != T_PIPE)
		{
			if (tokens[i].type != T_HERE_DOC)
				if (dollar_expand(&tokens[i], shell) == false || shell->f_malloc == 1)
				{
					free_tokens(tokens);
					fmalloc(shell, "expander", 2);
				}
		}
	}
	amount = new_tokens_amount(tokens, 0, 0, &shell->f_malloc);
	if (shell->f_malloc == 1)
	{
		free_tokens(tokens);
		fmalloc (shell, "new_tokens_amount", 2);
	}
	if (amount < 0)
		return (printf("ambiguous redirect stoopid D:<\n"), NULL);
	new_tokens = create_new_tokens(tokens, amount, 0, shell);
	if (shell->f_malloc == 1)
	{
		free_tokens(tokens);
		if (new_tokens)
			free_tokens(new_tokens);
		fmalloc (shell, "create_tokens_amount", 2);
	}
	i = -1;
	while (new_tokens && new_tokens[++i].type != T_NULL)
		if (new_tokens[i].type != T_PIPE)
		{
			remove_quotes(&new_tokens[i], shell);
			if (shell->f_malloc == 1)
			{
				free_tokens(tokens);
				if (new_tokens)
					free_tokens(tokens);
				fmalloc(shell, "create_tokens_amount", 2);
			}
		}
	return (new_tokens);
}
