/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:42:40 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/04/09 19:45:02 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/mini_header.h"

void	build_exit(t_mini *mini)
{
	free(mini->input);
	if (mini->pwd)
		free(mini->pwd);
	free(mini->env->home);
	freetrix(mini->env->my_env);
	free(mini->env);
	clear_history();
	exit(0);
}

void	build_echo(t_mini *mini)
{
	if (ft_strncmp(mini->input + 5, "-n ", 3) == 0)
		ft_printf("%s", mini->input + 8);
	else
		ft_printf("%s\n", mini->input + 5);
}

int	build_env(t_mini *mini)
{
	int	i;

	i = -1;
	while (mini->env->my_env[++i])
		ft_printf("%s\n", mini->env->my_env[i]);
	return (1);
}
int	build_unset(t_mini *mini)
{
	int	i;

	i = -1;
	while (mini->env->my_env[++i])
		if (ft_strnstr(mini->env->my_env[i],mini->input + 6,ft_strlen(mini->input + 6)))
			break ;
	
	printf("UNSETTING SOMETHING BITCH\n%s\n", mini->env->my_env[i]);
	while (mini->env->my_env[i] != NULL && mini->env->my_env[i + 1])
	{
		free(mini->env->my_env[i]);
		mini->env->my_env[i] = ft_strdup(mini->env->my_env[i + 1]);
		i++;
	}
	return (1);
}
// build_export();
int	build_pwd(t_mini *mini)
{
	ft_printf("%s\n", mini->pwd);
	return (1);
}

void	get_pwd(t_mini *mini)
{
	char	*temp;
	char	*cdw;

	temp = NULL;
	cdw = getcwd(temp, 100);
	free(mini->pwd);
	mini->pwd = ft_strjoin(cdw, "");
	free(temp);
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
	int		i;

	i = 3;
	if (!ft_strcmp(mini->input, "cd"))
	{
		chdir(mini->env->home);
		get_pwd(mini);
	}
	else
	{
		cd = ft_strjoin(mini->pwd, "/");
		cd2 = ft_strjoin(cd, mini->input + 3);
		if (chdir(cd2) < 0)
			ft_printf("Minishell: cd: %s: No such file or directory\n",
				mini->input + 3);
		get_pwd(mini);
		free(cd2);
		free(cd);
	}
	pwd_update(mini);
	ft_printf("Mini path = %s\n", mini->pwd);
	return (1);
}
