
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

// #include "../../incs/mini_header.h"

char	*find_env(t_token *token, t_mini shell, int *i)
{
	char	*expand;
	int		count;
	int		j;

	j = 0;
	count = 0;
	while ((*token).value[*i] && ft_isalnum((*token).value[*i]))
	{
		(*i)++;
		count++;
	}
	expand = ft_calloc(count + 2, sizeof(char));
	if (expand == NULL)
		return (NULL);
	ft_strlcpy(expand, (*token).value + *i - count, count + 1);
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

char	*found_dollar(t_token *token, t_mini shell, int *i)
{
	char	*expand;

	expand = NULL;
	if ((*token).value[*i] == '$')
	{
		(*i)++;
		if ((*token).value[*i] == '?')
		{
			expand = status_expand(shell);
			if (expand == NULL)
				return (NULL);
			(*i)++;
		}
		else if (ft_isalnum((*token).value[*i]))
		{
			expand = find_env(token, shell, i);
			if (expand == NULL)
				return (NULL);
		}
		else
			return ("$");
	}
	return (expand);
}

void	put_expansion(t_token *token, t_mini shell, char *expand, int amount)
{
	int		i;
	int		j;
	int		k;
	char	*temp;

	i = 0;
	j = 0;
	while ((*token).value[i] && amount > j)
	{
		temp = NULL;
		if ((*token).value[i] == '\'')
		{
			expand[j] = (*token).value[i];
			j++;
			i++;
			while ((*token).value[i] && (*token).value[i] != '\'')
			{
				expand[j] = (*token).value[i];
				j++;
				i++;
			}
			expand[j] = (*token).value[i];
			j++;
			i++;
		}
		else
		{
			if ((*token).value[i] == '\"')
			{
				expand[j] = (*token).value[i];
				j++;
				i++;
				while ((*token).value[i] && (*token).value[i] != '\"')
				{
					if ((*token).value[i] && (*token).value[i] == '$')
					{
						temp = found_dollar(token, shell, &i);
						if (temp != NULL)
						{
							k = 0;
							while(temp[k])
							{
								expand[j] = temp[k];
								j++;
								k++;
							}
							// ft_strlcpy(expand + j, src, ft_strlen(temp) + 1);
						}
					}
					else
					{
						expand[j] = (*token).value[i];
						j++;
						i++;
					}
				}
				expand[j] = (*token).value[i];
				j++;
				i++;
			}
			else if ((*token).value[i] == '$')
			{
				temp = found_dollar(token, shell, &i);
				if (temp == NULL)
					continue;
				k = 0;
				while(temp[k])
				{
					expand[j] = temp[k];
					j++;
					k++;
				}
			}
			else
			{
				expand[j] = (*token).value[i];
				i++;
				j++;
			}
		}
	}
}

int	get_amount(t_token *token, t_mini shell)
{
	char	*expand;
	int		amount;
	int		i;

	amount = 0;
	i = 0;
	while ((*token).value[i])
	{
		expand = NULL;
		if ((*token).value[i] == '\'')
		{
			i++;
			amount++;
			while ((*token).value[i] && (*token).value[i] != '\'')
			{
				i++;
				amount++;
			}
			i++;
			amount++;
		}
		else
		{
			if ((*token).value[i] == '\"')
			{
				i++;
				amount++;
				while ((*token).value[i] && (*token).value[i] != '\"')
				{
					if ((*token).value[i] && (*token).value[i] == '$')
					{
						expand = found_dollar(token, shell, &i);
						if (expand != NULL)
							amount += ft_strlen(expand);
					}
					else
					{
						i++;
						amount++;
					}
				}
				i++;
				amount++;
			}
			else if ((*token).value[i] == '$')
			{
				expand = found_dollar(token, shell, &i);
				if (expand == NULL)
					continue;
				amount += ft_strlen(expand);
			}
			else
			{
				i++;
				amount++;
			}
		}
	}
	return (amount);
}

bool	dollar_expand(t_token *token, t_mini shell)
{
	char	*expand;
	int		amount;

	amount = get_amount(token, shell);
	expand = ft_calloc(amount + 1, sizeof(char));
	if ((*token).value == NULL)
		return (false);
	expand[amount] = '\0';
	put_expansion(token, shell, expand, amount);
	free((*token).value);
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