/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:33:33 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/04/30 18:44:48 by dgarcez-         ###   ########.fr       */
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
	// free(cdw);
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
	if(mini->env->my_env[i])
	{
		free(mini->env->my_env[i]);
		mini->env->my_env[i] = ft_strjoin(prefix, mini->pwd);
	}
	// free(prefix);
}

static int cd_home(t_mini *mini)
{
	if(mini->env->home != NULL)
	{
		chdir(mini->env->home);
		get_pwd(mini);
	}
	else
		ft_putstr_fd("Minishell: cd: HOME not set\n",2);
	return(1);
}

static char *get_dir(t_cmd cmds,char *buffer,char *cd2,char *pwd)
{
	if (cmds.args[1][0] == '/')
	{
		cd2 = ft_strdup(cmds.args[0]);
		free(pwd);
	}
	else
	{
		buffer = ft_strjoin(pwd, "/");
		cd2 = ft_strjoin(buffer,cmds.args[1]);
	}
	// free(buffer);
	return (cd2);
}
int	build_cd(t_mini *mini,t_cmd cmds)
{
	char	*cd2;
	char 	*pwd;
	char *buffer;
	int t;

	buffer = NULL;
	cd2 = NULL;
	if(cmds.amount > 2)
		return(ft_printf("Minishell: cd: too many arguments\n"),1);
	if (!cmds.args[1])
		return (cd_home(mini));
	pwd = ft_strdup(mini->pwd);
	do_redirect(&cmds,&t);
	cd2 = get_dir(cmds,buffer,cd2,pwd);
	if (chdir(cd2) < 0 && cd2)
		ft_printf("Minishell: cd: %s: No such file or directory\n",cmds.args[0]);
	else
		get_pwd(mini);
	free(cd2);
	// free(pwd);
	return (pwd_update(mini), 1);
}
