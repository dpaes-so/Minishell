/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:32:35 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/04/14 16:19:01 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

int	print_env_ex(t_mini *mini)
{
	int	i;

	i = -1;
	while (mini->env->my_env[++i])
		ft_printf("declare -x %s\n", mini->env->my_env[i]);
	return (1);
}

int	check_valid_variable_name(char *s)
{
	int	i;

	i = 0;
	if (s[0] == '=')
		return (0);
	if (!((s[0] >= 'a' && s[0] <= 'z') || (s[0] >= 'A' && s[0] <= 'Z')
			|| (s[0] == '_')))
		return (0);
	while (s[i] && s[i] != '=')
	{
		if (!((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')
				|| (s[i] >= '0' && s[i] <= '9') || (s[i] == '_')))
			return (0);
		i++;
	}
	return (i);
}

char	*ft_strchr(const char *str, int c)
{
	int		i;
	char	*s;

	i = 0;
	s = (char *)str;
	while (s[i])
	{
		if (str[i] == (char)c)
			return (s + i);
		i++;
	}
	return (NULL);
}

void	*make_export(t_mini *mini, char *arg)
{
	char	**new_env;
	int		size;
	int		break_point;
	int		i;

	size = 0;
	i = -1;
	break_point = -1;
	while (mini->env->my_env[size])
		size++;
	new_env = malloc(sizeof(char *) * (size + 2));
	if (!new_env)
		return (NULL);
	while (mini->env->my_env[++break_point])
		if (!ft_strncmp(mini->env->my_env[break_point], arg, ft_strchr(arg, '=')- arg))
			return (free(mini->env->my_env[break_point]),mini->env->my_env[break_point] = ft_strdup(arg),NULL);
	while (++i < size)
		new_env[i] = mini->env->my_env[i];
	new_env[i] = arg;
	new_env[++i] = NULL;
	free(mini->env->my_env);
	mini->env->my_env = new_env;
	return (NULL);
}

int	build_export(t_mini *mini)
{
	char	*arg;
	char	**split;
	int		i;
	int		split_index;

	if (!ft_strcmp(mini->input, "export") || !ft_strcmp(mini->input, "export "))
		return (print_env_ex(mini));
	split_index = 0;
	split = ft_split(mini->input + 7, ' ');
	while (split[split_index])
	{
		arg = split[split_index];
		i = check_valid_variable_name(arg);
		ft_printf("!%s!\n", arg);
		if (!i)
		{
			ft_printf("Minishell: '%s' not a valid identifier\n", arg);
			split_index++;
			continue ;
		}
		make_export(mini, arg);
		split++;
	}
	freetrix(split);
	return (1);
}
