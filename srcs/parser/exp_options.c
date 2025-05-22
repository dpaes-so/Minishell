/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_options.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniel <daniel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 02:21:31 by daniel            #+#    #+#             */
/*   Updated: 2025/05/21 02:22:15 by daniel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

char	*status_expand(t_mini *shell)
{
	char	*result;

	result = ft_itoa(shell->pipex.status);
	if (result == NULL)
		return (NULL);
	return (result);
}

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

char	*no_dollar(t_token token)
{
	int	count;

	count = unclosed_quotes(token);
	if (count % 2 != 0 || count == 0 || *token.value == '$')
		return ("$");
	return (NULL);
}
