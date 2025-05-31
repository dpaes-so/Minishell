/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_exp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniel <daniel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:22:31 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/06/01 00:40:55 by daniel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

// int		skip_space(char	*env)
// {
// 	int	i;

// 	i = 0;
// 	while((env[i] && env[i] == ' ') || (env[i] >= 9 && env[i] <= 13))
// 		i++;
// 	return (i);
// }

void	handle_dollar(t_token *token, t_mini *shell, char *expand, int *j)
{
	int		flag;
	char	*temp;

	flag = 0;
	temp = found_dollar(token, shell, &flag);
	if (temp != NULL && temp[0])
	{
		if ((*token).in_quotes == false)
			temp = add_quotes(temp, &flag, shell);
		if (expand != NULL && temp != NULL)
			ft_strlcpy(expand + *j, temp, ft_strlen(temp) + 1);
		*j += ft_strlen(temp);
	}
	if (temp && (flag == 1 || flag == 2))
		free(temp);
}

void	put_expansion(t_token *token, t_mini *shell, char *expand, int amount)
{
	int	j;

	j = 0;
	while (*(*token).value || amount > j)
	{
		if (*(*token).value == '\'')
			handle_s_quote(token, expand, &j);
		else
		{
			if (*(*token).value == '\"')
				handle_d_quote(token, shell, expand, &j);
			else if (*(*token).value == '$')
				handle_dollar(token, shell, expand, &j);
			else
				small_cpy(token, expand, &j, 0);
		}
	}
}

int	get_amount(t_token *token, t_mini *shell)
{
	int	amount;

	amount = 0;
	while (token && token->value && *(*token).value)
	{
		if (*(*token).value == '\'')
		{
			small_cpy(token, NULL, 0, &amount);
			while (*(*token).value && *(*token).value != '\'')
				small_cpy(token, NULL, 0, &amount);
			small_cpy(token, NULL, 0, &amount);
		}
		else
		{
			if (*(*token).value == '\"')
				handle_d_quote(token, shell, NULL, &amount);
			else if (*(*token).value == '$')
				handle_dollar(token, shell, NULL, &amount);
			else
				small_cpy(token, NULL, 0, &amount);
		}
	}
	return (amount);
}

bool	dollar_expand(t_token *token, t_mini *shell)
{
	char	*expand;
	int		amount;
	int		len;

	len = ft_strlen((*token).value);
	amount = get_amount(token, shell);
	if (amount == 0)
	{
		free((*token).value - len);
		(*token).value = NULL;
		return (true);
	}
	token->value = token->value - len;
	expand = ft_calloc(amount + 1, sizeof(char));
	if (expand == NULL)
		return (false);
	expand[amount] = '\0';
	put_expansion(token, shell, expand, amount);
	free((*token).value - len);
	(*token).value = ft_strdup(expand);
	if (!(*token).value)
		return (free(expand), false);
	free(expand);
	return (true);
}
