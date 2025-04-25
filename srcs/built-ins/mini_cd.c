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

	cdw = NULL;
	cdw = getcwd(cdw, 100);
	free(mini->pwd);
	mini->pwd = ft_strjoin(cdw, "");
}

void	pwd_update(t_mini *mini)
{
	int	i;
	char *prefix;


	i = -1;
	prefix = ft_strdup("PWD=");
	while (mini->env->my_env[++i])
		if (ft_strnstr(mini->env->my_env[i], "PWD=", 4))
			break ;
	free(mini->env->my_env[i]);
	mini->env->my_env[i] = ft_strjoin(prefix, mini->pwd);
}
int	build_cd(t_mini *mini,t_cmd cmds)
{
	char	*cd2;
	char 	*pwd;

	if(cmds.amount > 1)
		return(ft_printf("Minishell: cd: too many arguments\n"),1);
	if (!cmds.args[0])
		return (chdir(mini->env->home), get_pwd(mini), (1));
	pwd = ft_strdup(mini->pwd);
	do_redirect(cmds);
	if (cmds.args[0][0] == '/')
	{
		cd2 = ft_strdup(cmds.args[0]);
		free(pwd);
	}
	else
		cd2 = ft_strjoin(ft_strjoin(pwd, "/"),cmds.args[0]);
	if (chdir(cd2) < 0)
		ft_printf("Minishell: cd: %s: No such file or directory\n",cmds.args[0]);
	else
		get_pwd(mini);
	if(cd2)
		free(cd2);
	return (pwd_update(mini), ft_printf("Mini path = %s\n", mini->pwd), 1);
}
