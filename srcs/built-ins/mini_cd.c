/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:33:33 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/04/14 16:06:32 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

void	get_pwd(t_mini *mini)
{
	char	*cdw;

	cdw = getcwd(NULL, 100);
	free(mini->pwd);
	mini->pwd = ft_strjoin(cdw, "");
	free(cdw);
}

void	pwd_update(t_mini *mini)
{
	int	i;

	i = -1;
	while (mini->env->my_env[++i])
		if (ft_strnstr(mini->env->my_env[i], "PWD=", 4))
			break ;
	free(mini->env->my_env[i]);
	mini->env->my_env[i] = ft_strjoin("PWD=", mini->pwd);
}

int	build_cd(t_mini *mini)
{
	char	*cd;
	char	*cd2;
	char	*arg;

	arg = mini->input + 3;
	if (!ft_strcmp(mini->input, "cd"))
		return (chdir(mini->env->home), get_pwd(mini), (1));
	else
	{
		if (arg[0] == '/')
			cd2 = ft_strdup(mini->input + 3);
		else
		{
			cd = ft_strjoin(mini->pwd, "/");
			cd2 = ft_strjoin(cd, mini->input + 3);
			free(cd);
		}
		if (chdir(cd2) < 0)
			ft_printf("Minishell: cd: %s: No such file or directory\n",
				mini->input + 3);
		else
			get_pwd(mini);
		free(cd2);
	}
	return (pwd_update(mini), ft_printf("Mini path = %s\n", mini->pwd), 1);
}
