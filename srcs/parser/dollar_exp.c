/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_exp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:22:31 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/05/02 17:27:11 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

char	*find_env(t_token *token, t_mini *shell)
{
	char	*expand;
	int		count;
	int		j;

	j = 0;
	count = 0;
	while (*(*token).value && ft_isalnum(*(*token).value))
	{
		(*token).value++;
		count++;
	}
	expand = ft_calloc(count + 2, sizeof(char));
	if (expand == NULL)
		return (NULL);
	ft_strlcpy(expand, (*token).value - count, count + 1);
	expand = ft_strjoin(expand, "=");
	while (shell->env->my_env[j])
	{
		if (ft_strnstr(shell->env->my_env[j], expand, count + 1))
			return (free(expand), shell->env->my_env[j] + count + 1);
		j++;
	}
	free(expand);
	return (NULL);
}

void	handle_dollar(t_token *token, t_mini *shell, char *expand, int *j)
{
	int		flag;
	char	*temp;

	flag = 0;
	temp = found_dollar(token, shell, &flag);
	if (temp != NULL)
	{
		if (expand != NULL)
			ft_strlcpy(expand + *j, temp, ft_strlen(temp) + 1);
		*j += ft_strlen(temp);
	}
	if (flag == 1 && temp)
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
	while (*(*token).value)
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
	expand = ft_calloc(amount + 1, sizeof(char));
	if ((*token).value == NULL)
		return (false);
	expand[amount] = '\0';
	token->value = token->value - len;
	put_expansion(token, shell, expand, amount);
	free((*token).value - len);
	(*token).value = ft_strdup(expand);
	free(expand);
	return (true);
}
