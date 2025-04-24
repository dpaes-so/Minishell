/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:58:38 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/04/24 20:51:11 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

int	find_env(t_mini shell, char *expand, int remove)
{
	int	i;

	i = 0;
	while (shell.env->my_env[i])
	{
		if (ft_strnstr(shell.env->my_env[i], expand, remove))
			return (ft_strlen(shell.env->my_env[i]));
		i++;
	}
}

int	get_amount_and_remove(t_token *token, t_mini shell, int *i, int *remove)
{
	char	*expand;

	if ((*token).value[*i] == '$')
	{
		(*i)++;
		if ((*token).value[*i])
			(*remove)++;
		// if ((*token).value[*i] == '?')
		// return (func)
		while ((*token).value[*i] && (*token).value[*i] != '$'
			&& (*token).value[*i] != '?')
		{
			(*remove)++;
			(*i)++;
		}
		expand = ft_calloc(*remove + 1, sizeof(char));
		ft_strlcpy(expand, (*token).value, *i - *remove);
		find_env(shell, expand, remove);
		free(expand);
	}
	else
		(*i)++;
	return (0);
}

bool	dollar_expand(t_token *token, t_mini shell)
{
	char	*expand;
	int		amount;
	int		remove;
	int		i;
	int		j;

	i = 0;
	remove = 0;
	amount = 0;
	while ((*token).value[i])
		amount += get_amount_and_remove(token, shell, &i, &remove);
	free((*token).value);
	(*token).value = ft_calloc((ft_strlen((*token).value) + amount - remove)
			+ 1, sizeof(char));
	if ((*token).value == NULL)
		return (false);
	i = 0;
	(*token).value[ft_strlen((*token).value) + amount - remove] = '\0';
	if (count == 0)
		return (false);
	return (true);
}
/* 
bool	remove_quotes(t_token *token, t_mini shell)
{
	int		i;
	int		j;
	int		count;
	char	*temp;
	char	quote;

	(void)shell;
	
	temp = ft_strdup((*token).value);
	if (temp == NULL)
		return (false);
	count = 0;
	i = 0;
	j = 0;
	while((*token).value[i])
	{
		if((*token).value[i] == '\'' || (*token).value[i] == '\"')
		{
			quote = (*token).value[i];
			i++;
			while((*token).value[i] != '\0' && (*token).value[i] != quote)
				i++;
			if ((*token).value[i] != '\0' && (*token).value[i] == quote)
				count += 2;
		}
/* 		if ((*token).value[i] == '$')
			env_expansion(token->value, shell, i + 1); */
i++;
}
free((*token).value);
(*token).value = ft_calloc((ft_strlen(temp) - count) + 1, sizeof(char));
if	((*token).value == NULL)
	return (false);
i = 0;
while (temp[i])
{
	if (temp[i] == '\'' || temp[i] == '\"')
	{
		quote = temp[i];
		i++;
		while (temp[i] != '\0' && temp[i] != quote)
		{
			(*token).value[j] = temp[i];
			j++;
			i++;
		}
	}
	else
	{
		(*token).value[j] = temp[i];
		j++;
	}
	i++;
}
(*token).value[ft_strlen(temp) - count] = '\0';
free(temp);
if (count == 0)
	return (false);
return (true);
}
* /

	void expand_strs(t_token *tokens, t_mini shell)
{
	(void)shell;
	int i;

	i = 0;
	while (tokens[i].type != T_NULL)
	{
		if (tokens[i].type != T_PIPE)
		{
			remove_quotes(&tokens[i], shell);
		}
		i++;
	}
}