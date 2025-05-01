
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:58:38 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/04/30 17:42:32 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

char	*find_env(t_token *token, t_mini shell)
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
	while (shell.env->my_env[j])
	{
		if (ft_strnstr(shell.env->my_env[j], expand, count + 1))
		{
			free(expand);
			return (shell.env->my_env[j] + count + 1);
		}
		j++;
	}
	free(expand);
	return (NULL);
}

char	*status_expand(t_mini shell)
{
	char	*result;

	result = ft_itoa(shell.pipex.status);
	if (result == NULL)
		return (NULL);
	return (result);
}

char	*found_dollar(t_token *token, t_mini shell, int *flag)
{
	char	*expand;

	expand = NULL;
	if (*(*token).value == '$')
	{
		(*token).value++;
		if (*(*token).value == '?')
		{
			expand = status_expand(shell);
			if (expand == NULL)
				return (NULL);
			*flag = 1;
			(*token).value++;
		}
		else if (ft_isalnum(*(*token).value))
		{
			expand = find_env(token, shell);
			if (expand == NULL)
				return (NULL);
		}
		else
			return ("$");
	}
	return (expand);
}

void	handle_s_quote(t_token *token, char *expand, int *j)
{
	expand[*j] = *(*token).value;
	(*j)++;
	(*token).value++;
	while (*(*token).value && *(*token).value != '\'')
	{
		expand[*j] = *(*token).value;
		(*j)++;
		(*token).value++;
	}
	expand[*j] = *(*token).value;
	(*j)++;
	(*token).value++;
}

void	small_cpy(t_token *token, char *expand, int *j, int *amount)
{
	if (expand != NULL)
	{
		expand[*j] = *(*token).value;
		(*j)++;
	}
	if (expand == NULL)
		(*amount)++;
	(*token).value++;
}

void	handle_dollar(t_token *token, t_mini shell, char *expand, int *j)
{
	int	flag;
	char *temp;

	flag = 0;
	temp = found_dollar(token, shell, &flag);
	if (temp != NULL)
	{
		if (expand != NULL)
			ft_strlcpy(expand + *j, temp, ft_strlen(temp) + 1);
		*j += ft_strlen(temp);
	}
	if (flag == 1)
		free(temp);
}

void	handle_d_quote(t_token *token, t_mini shell, char *expand, int *j)
{
	if (expand != NULL)
	{
		small_cpy(token, expand, j, 0);
		while (*(*token).value && *(*token).value != '\"')
		{
			if (*(*token).value && *(*token).value == '$')
				handle_dollar(token, shell, expand, j);
			else
				small_cpy(token, expand, j, 0);
		}
		small_cpy(token, expand, j, 0);
	}
	else
	{
		small_cpy(token, NULL, 0, j);
		while (*(*token).value && *(*token).value != '\"')
		{
			if (*(*token).value && *(*token).value == '$')
				handle_dollar(token, shell, NULL, j);
			else
				small_cpy(token, NULL, 0, j);
		}
		small_cpy(token, NULL, 0, j);
	}
}

void	put_expansion(t_token *token, t_mini shell, char *expand, int amount)
{
	int		j;

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

int	get_amount(t_token *token, t_mini shell)
{
	int		amount;

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

bool	dollar_expand(t_token *token, t_mini shell)
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

// bool	remove_quotes(t_token *token, t_mini shell)
// {
// 	int		i;
// 	int		j;
// 	int		count;
// 	char	*temp;
// 	char	quote;

// 	(void)shell;

// 	temp = ft_strdup((*token).value);
// 	if (temp == NULL)
// 		return (false);
// 	count = 0;
// 	i = 0;
// 	j = 0;
// 	while((*token).value[i])
// 	{
// 		if((*token).value[i] == '\'' || (*token).value[i] == '\"')
// 		{
// 			quote = (*token).value[i];
// 			i++;
// 			while((*token).value[i] != '\0' && (*token).value[i] != quote)
// 				i++;
// 			if ((*token).value[i] != '\0' && (*token).value[i] == quote)
// 				count += 2;
// 		}
// 		if ((*token).value[i] == '$')
// 			env_expansion(token->value, shell, i + 1);
// 		i++;
// 	}
// 	free((*token).value);
// 	(*token).value = ft_calloc((ft_strlen(temp) - count) + 1, sizeof(char));
// 	if	((*token).value == NULL)
// 		return (false);
// 	i = 0;
// 	while (temp[i])
// 	{
// 		if (temp[i] == '\'' || temp[i] == '\"')
// 		{
// 			quote = temp[i];
// 			i++;
// 			while (temp[i] != '\0' && temp[i] != quote)
// 			{
// 				(*token).value[j] = temp[i];
// 				j++;
// 				i++;
// 			}
// 		}
// 		else
// 		{
// 			(*token).value[j] = temp[i];
// 			j++;
// 		}
// 		i++;
// 	}
// 	(*token).value[ft_strlen(temp) - count] = '\0';
// 	free(temp);
// 	if (count == 0)
// 		return (false);
// 	return (true);
// }

void	expand_strs(t_token *tokens, t_mini shell)
{
	(void)shell;
	int i;

	i = 0;
	while (tokens[i].type != T_NULL)
	{
		if (tokens[i].type != T_PIPE || tokens[i].type != T_HERE_DOC)
		{
			dollar_expand(&tokens[i], shell);
			// remove_quotes(&tokens[i], shell);
		}
		i++;
	}
}