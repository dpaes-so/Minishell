/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- < dgarcez-@student.42lisboa.com > +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:33:33 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/06/04 13:31:40 by dgarcez-         ###   ########.fr       */
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
		return (set_fmalloc(shell));
	ft_strlcpy(expand, str - count, count + 1);
	expand = ft_strjoin(expand, "=");
	while (shell->env->my_env && shell->env->my_env[++j])
		if (ft_strnstr(shell->env->my_env[j], expand, count + 1))
			return (free(expand), shell->env->my_env[j] + count + 1);
	return (free(expand), NULL);
}

void	pwd_update(t_mini *mini)
{
	int		i;
	char	*prefix;

	i = -1;
	if (!mini->env)
		return ;
	while (mini->env->my_env && mini->env->my_env[++i])
	{
		if (ft_strnstr(mini->env->my_env[i], "PWD=", 4))
			break ;
	}
	if (mini->env->my_env[i])
	{
		prefix = ft_strdup("PWD=");
		free(mini->env->my_env[i]);
		mini->env->my_env[i] = ft_strjoin(prefix, mini->pwd);
		if (!prefix || !mini->env->my_env[i])
			fmalloc(mini, "pwd_update", 2);
	}
}

static int	cd_home(t_mini *mini)
{
	char	*home;

	home = find_in_env("HOME", mini);
	if (home != NULL)
	{
		chdir(home);
		get_pwd(mini);
	}
	else
	{
		ft_putstr_fd("Minishell: cd: HOME not set\n", 2);
		mini->pipex.status = 1;
	}
	return (1);
}

static char	*get_dir(t_cmd cmds, char *buffer, char *cd2, t_mini *mini)
{
	char	*pwd;

	pwd = ft_strdup(mini->pwd);
	if (!pwd)
		fmalloc(mini, "get_dir", 2);
	if (cmds.args[1][0] == '/')
	{
		cd2 = ft_strdup(cmds.args[1]);
		if (pwd)
			free(pwd);
	}
	else
	{
		buffer = ft_strjoin(pwd, "/");
		if (!buffer)
			fmalloc(mini, "get_dir", 2);
		cd2 = ft_strjoin(buffer, cmds.args[1]);
	}
	if (!cd2)
		fmalloc(mini, "get_dir", 2);
	return (cd2);
}

int	build_cd(t_mini *mini, t_cmd cmds)
{
	char	*cd2;
	char	*buffer;

	mini->pipex.status = 0;
	buffer = NULL;
	cd2 = NULL;
	if (mini->cmd_amount == 1)
		mini->wait_check = 0;
	if (cmds.amount > 2)
		return (mini->pipex.status = 1, ft_dprintf(2,
				"Minishell: cd: too many arguments\n"), 1);
	if (do_redirect(&cmds, mini) < 0)
		return (mini->pipex.status = 1, 1);
	if (!cmds.args[1])
		return (cd_home(mini));
	cd2 = get_dir(cmds, buffer, cd2, mini);
	if (chdir(cd2) < 0 && cd2)
	{
		ft_dprintf(2, "Minishell: cd: %s: No such file or directory\n",
			cmds.args[1]);
		mini->pipex.status = 1;
	}
	else
		get_pwd(mini);
	return (free(cd2), pwd_update(mini), 1);
}
