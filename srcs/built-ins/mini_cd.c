/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:33:33 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/16 15:52:57 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"


char	*find_in_env(char *str, t_mini *shell)
{
	char	*expand;
	int		count;
	int		j;

	j = -1;
	count = 0;
	if (ft_isdigit(*str))
	{
		str++;
		return (NULL);
	}
	while (str && (ft_isalnum(*str) || *str == '_'))
	{
		str++;
		count++;
	}
	expand = ft_calloc(count + 2, sizeof(char));
	if (expand == NULL)
		return (NULL);
	ft_strlcpy(expand, str - count, count + 1);
	expand = ft_strjoin(expand, "=");
	while (shell->env->my_env[++j])
		if (ft_strnstr(shell->env->my_env[j], expand, count + 1))
			return (free(expand), shell->env->my_env[j] + count + 1);
	return (free(expand), NULL);
}

void	pwd_update(t_mini *mini)
{
	int		i;
	char	*prefix;

	i = -1;
	while (mini->env->my_env[++i])
		if (ft_strnstr(mini->env->my_env[i], "PWD=", 4))
			break ;
	if (mini->env->my_env[i])
	{
		prefix = ft_strdup("PWD=");
		free(mini->env->my_env[i]);
		mini->env->my_env[i] = ft_strjoin(prefix, mini->pwd);
	}
}

static int	cd_home(t_mini *mini)
{
	char *home;
	
	home = find_in_env("HOME",mini);
	if (home != NULL)
	{
		chdir(home);
		get_pwd(mini);
	}
	else
		ft_putstr_fd("Minishell: cd: HOME not set\n", 2);
	return (1);
}

static char	*get_dir(t_cmd cmds, char *buffer, char *cd2, char *pwd)
{
	if (cmds.args[1][0] == '/')
	{
		cd2 = ft_strdup(cmds.args[1]);
		free(pwd);
	}
	else
	{
		buffer = ft_strjoin(pwd, "/");
		cd2 = ft_strjoin(buffer, cmds.args[1]);
		ft_printf("after join !%s!\n", cd2);
	}
	return (cd2);
}

int	build_cd(t_mini *mini, t_cmd cmds)
{
	char	*cd2;
	char	*pwd;
	char	*buffer;

	buffer = NULL;
	cd2 = NULL;
	if (cmds.amount > 2)
		return (ft_printf("Minishell: cd: too many arguments\n"), 1);
	do_redirect(&cmds, mini);
	if (!cmds.args[1])
		return (cd_home(mini));
	pwd = ft_strdup(mini->pwd);
	cd2 = get_dir(cmds, buffer, cd2, pwd);
	ft_printf("tryng to go to !%s!\n", cd2);
	if (chdir(cd2) < 0 && cd2)
	{
		ft_printf("Minishell: cd: %s: No such file or directory\n",
			cmds.args[1]);
			mini->pipex.status = 1;
	}
	else
		get_pwd(mini);
	free(cd2);
	return (pwd_update(mini), 1);
}
