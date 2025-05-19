/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_more_exec_aux.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:33:17 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/13 17:34:21 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

void	set_shlvl(t_mini *mini)
{
	int		sh_lvl;
	char	*temp;
	char	*shlvl;
	int		i;

	i = -1;
	while (mini->env->my_env[++i])
		if (ft_strnstr(mini->env->my_env[i], "SHLVL=", 6))
			break ;
	sh_lvl = ft_atoi(mini->env->my_env[i] + 6);
	sh_lvl++;
	shlvl = ft_itoa(sh_lvl);
	temp = ft_strjoin(ft_strdup("SHLVL="), shlvl);
	free(mini->env->my_env[i]);
	mini->env->my_env[i] = ft_strdup(temp);
	free(temp);
	free(shlvl);
}
static void	my_env_continue(t_mini *mini, char **ev)
{
	int i;

	i = -1;
	mini->env->my_env = ft_matrix_dup(mini->env->my_env,ev);
	if (mini->env->my_env == NULL)
		return ;
	mini->env->my_export = ft_matrix_dup(mini->env->my_export,ev);
	if (mini->env->my_export == NULL)
		return ;
	while (ev[++i])
		if (ft_strnstr(ev[i], "HOME=", 5))
			break ;
	mini->env->home = NULL;
	if (ev[i])
		mini->env->home = ft_strdup(ev[i] + 5);
	if (mini->env->home == NULL)
		return ;
	ft_sort_matrix(mini->env->my_export);
	set_shlvl(mini);
}
void	my_env_start(t_mini *mini, char **ev)
{
	int	k;

	k = 0;
	mini->env = malloc(sizeof(t_env));
	if (mini->env == NULL)
		return ;
	while (ev[k])
		k++;
	mini->env->my_env = (char **)ft_calloc(k + 1, sizeof(char *));
	if (mini->env->my_env == NULL)
		return ;
	mini->env->my_export = (char **)ft_calloc(k + 1, sizeof(char *));
	if (mini->env->my_env == NULL)
		return ;
	my_env_continue(mini,ev);
}

t_mini	*mem_save(t_mini *to_save)
{
	static t_mini	*save;

	if (to_save)
		save = to_save;
	return (save);
}
