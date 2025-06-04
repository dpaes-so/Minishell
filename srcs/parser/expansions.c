/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:58:38 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/06/04 11:39:34 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

int	new_tokens_amount(t_token *tokens, int i, int j, int *flag)
{
	int		count;
	char	**res;
	int		amount;

	amount = 0;
	while (tokens[++i].type != T_NULL)
	{
		j = 0;
		count = ambiguous_check(tokens, i);
		res = ft_arg_split(tokens[i].value + count, ' ', flag);
		if (res == NULL || tokens[i].ambiguous == true)
			amount++;
		while (res && res[j] && tokens[i].ambiguous == false)
			j++;
		if (tokens[i].type >= T_HERE_DOC && tokens[i].type <= T_APPEND_REDIR
			&& j > 1)
		{
			tokens[i].ambiguous = true;
			j = 1;
		}
		amount += j;
		freetrix(res);
	}
	return (amount);
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
	while (tokens[i].type != T_NULL)
	{
		if (!(tokens[i].type >= T_HERE_DOC && tokens[i].type <= T_APPEND_REDIR))
		{
			if (put_new_tokens(tokens, new_tokens, &k, &i) == 1)
			{
				shell->f_malloc = 1;
				return (free_tokens(new_tokens), NULL);
			}
		}
		else
			put_redir_tokens(tokens, new_tokens, &k, &i);
	}
	return (new_tokens);
}

void	expander(t_token *tokens, t_mini *shell)
{
	int	i;

	i = -1;
	while (tokens[++i].type != T_NULL)
	{
		if (tokens[i].type != T_PIPE)
		{
			if (tokens[i].type != T_HERE_DOC)
			{
				if (dollar_expand(&tokens[i], shell) == false
					|| shell->f_malloc == 1)
				{
					free_tokens(tokens);
					fmalloc(shell, "expander", 2);
				}
			}
		}
	}
}

void	quoter(t_token *tokens, t_token *new_tokens, t_mini *shell)
{
	int	i;

	i = -1;
	while (new_tokens && new_tokens[++i].type != T_NULL)
	{
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
	}
}

t_token	*expand_strs(t_token *tokens, t_mini *shell)
{
	int		amount;
	t_token	*new_tokens;

	expander(tokens, shell);
	amount = new_tokens_amount(tokens, -1, 0, &shell->f_malloc);
	if (shell->f_malloc == 1)
	{
		free_tokens(tokens);
		fmalloc (shell, "new_tokens_amount", 2);
	}
	new_tokens = create_new_tokens(tokens, amount, 0, shell);
	if (shell->f_malloc == 1)
	{
		free_tokens(tokens);
		fmalloc (shell, "create_tokens_amount", 2);
	}
	quoter(tokens, new_tokens, shell);
	return (new_tokens);
}
