/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 16:51:17 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/12 18:08:36 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

int	check_valid_variable_name(char *s)
{
	int	i;
	int	ctd;

	i = 0;
	ctd = 0;
	if (s[0] == '=')
		return (0);
	if (!((s[0] >= 'a' && s[0] <= 'z') || (s[0] >= 'A' && s[0] <= 'Z')
			|| (s[0] == '_')))
		return (0);
	while (s[i] && s[i] != '=')
	{
		if (!((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')
				|| (s[i] >= '0' && s[i] <= '9') || (s[i] == '_')
				|| s[i] == '+' || s[i] == '=') || (ctd == 1 && s[i] != '='))
			return (0);
		if (s[i] == '+')
			ctd++;
		if (ctd == 2)
			return (0);
		i++;
	}
	if(ctd == 1 && s[i] !=  '=')
		return(0);
	if (s[i - 1] == '+')
		return (2);
	return (1);
}

int	print_env_ex(t_mini *mini)
{
	int	i;

	i = -1;
	while (mini->env->my_export[++i])
		ft_printf("declare -x %s\n", mini->env->my_export[i]);
	return (1);
}

char *remove_plus_sign(char *src)
{
	char *new_arg;
	int i;
	int j;

	j = 0;
	i = 0;
	new_arg = ft_calloc(ft_strlen(src),sizeof(char));
	while(src[j])
	{
		if(src[j] != '+')
		{
			new_arg[i] = src[j];
			i++;
		}
		j++;
	}
	new_arg[i] = '\0';
	return(new_arg);
}

void	*add_exp_agn(t_mini *mini, char *arg)
{
	char	**new_export;
	int		size;
	int		break_point;
	int		i;
	int 	equal_check;

	size = 0;
	i = -1;
	break_point = -1;
	while (mini->env->my_export[size])
		size++;
	while (mini->env->my_export[++break_point])
		if (!ft_strncmp(mini->env->my_export[break_point], arg, ft_strchr(arg, '+')- arg))
		{
			equal_check = 0;
			if(ft_strchr(mini->env->my_export[break_point],'='))
				equal_check = 1;
			return (mini->env->my_export[break_point] = ft_strjoin(mini->env->my_export[break_point],ft_strchr(arg, '+') + equal_check + 1), NULL);
		}
	new_export = ft_calloc((size + 2), sizeof(char *));
	if (!new_export)
		return (NULL);
	while (mini->env->my_export[++i])
		new_export[i] = ft_strdup(mini->env->my_export[i]);
	arg = remove_plus_sign(arg);
	new_export[i++] = ft_strdup(arg);
	new_export[i] = NULL;
	freetrix(mini->env->my_export);
	return (free(arg),mini->env->my_export = new_export, NULL);
}

void	*add_export(t_mini *mini, char *arg)
{
	char	**new_env;
	int		size;
	char 	*new_arg;
	int		break_point;
	int		i;

	size = 0;
	i = -1;
	break_point = -1;
	while (mini->env->my_env[size])
		size++;
	while (mini->env->my_env[++break_point])
		if (!ft_strncmp(mini->env->my_env[break_point], arg, ft_strchr(arg, '+')
				- arg))
			return (mini->env->my_env[break_point] = ft_strjoin(mini->env->my_env[break_point],
					ft_strchr(arg, '+') + 2),add_exp_agn(mini,arg), NULL);
	ft_printf("arg  1 = %s\n",arg);
	new_env = ft_calloc((size + 2), sizeof(char *));
	if (!new_env)
		return (NULL);
	while (mini->env->my_env[++i])
		new_env[i] = ft_strdup(mini->env->my_env[i]);
	new_arg = remove_plus_sign(arg);
	new_env[i++] = ft_strdup(new_arg);
	new_env[i] = NULL;
	freetrix(mini->env->my_env);
	return (add_exp_agn(mini,arg),free(new_arg),mini->env->my_env = new_env, NULL);
}
