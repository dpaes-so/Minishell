/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_exp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:22:31 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/05/16 14:32:54 by dgarcez-         ###   ########.fr       */
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

char	*find_env(t_token *token, t_mini *shell)
{
	char	*expand;
	int		count;
	int		j;

	j = -1;
	count = 0;
	if (ft_isdigit(*(*token).value))
		return ((*token).value++, NULL);
	while (*(*token).value && (ft_isalnum(*(*token).value)
			|| *(*token).value == '_'))
	{
		(*token).value++;
		count++;
	}
	expand = ft_calloc(count + 2, sizeof(char));
	if (expand == NULL)
		return (NULL);
	ft_strlcpy(expand, (*token).value - count, count + 1);
	expand = ft_strjoin(expand, "=");
	while (shell->env->my_env[++j])
		if (ft_strnstr(shell->env->my_env[j], expand, count + 1))
			return (free(expand), shell->env->my_env[j] + count + 1);
	return (free(expand), NULL);
}

char	*add_quotes(char *expand, int flag)
{
	char	*new_expand;
	int		i;

	i = 0;
	new_expand = ft_calloc(ft_strlen(expand) + 3, sizeof(char));
	if (new_expand == NULL)
		return (NULL);
	new_expand[0] = '\"';
	new_expand[ft_strlen(expand) + 1] = '\"';
	while (expand[i])
	{
		new_expand[i + 1] = expand[i];
		i++;
	}
	i++;
	new_expand[i + 1] = '\0';
	if (flag == 1 && expand)
		free(expand);
	return (new_expand);
}


void	handle_dollar(t_token *token, t_mini *shell, char *expand, int *j)
{
	int		flag;
	char	*temp;

	flag = 0;
	temp = found_dollar(token, shell, &flag);
	if (temp != NULL && temp[0])
	{
		if (expand != NULL)
			ft_strlcpy(expand + *j, temp, ft_strlen(temp) + 1);
		*j += ft_strlen(temp);
	}
	if(temp && flag == 1)
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
	if (amount == 0)
	{
		free((*token).value - len);
		(*token).value = NULL;
		return (true);
	}
	expand = ft_calloc(amount + 1, sizeof(char));
	if (expand == NULL)
		return (false);
	expand[amount] = '\0';
	token->value = token->value - len;
	put_expansion(token, shell, expand, amount);
	free((*token).value - len);
	(*token).value = ft_strdup(expand);
	free(expand);
	return (true);
}
