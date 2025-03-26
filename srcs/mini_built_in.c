/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:42:40 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/03/26 19:38:27 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../incs/mini_header.h"

void	build_exit(t_mini *mini)
{
	free(mini->input);
	if (mini->path)
		free(mini->path);
	free(mini->my_env);
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

int build_env(t_mini *mini)
{
	int i;

	i = -1;
	ft_printf("AYOOOOOO MY ENV");
	while(mini->my_env->my_env[++i])
		ft_printf("%s\n",mini->my_env->my_env[i]);
	ft_printf("AYOOOOOO MY ENV ENDED");
	return(1);
}
// build_unset();
// build_export();
// build_pwd();



void get_pwd(t_mini *mini)
{
	char	*temp;
	char	*cdw;
	
	temp = NULL;
	cdw = getcwd(temp, 100);
	free(mini->path);
	mini->path = ft_strjoin(cdw,"");
	free(temp);
	free(cdw);
}
void pwd_update(t_mini *mini)
{
	int i;

	i = -1;
	while(mini->my_env->my_env[++i])
		if (ft_strnstr(mini->my_env->my_env[i], "PWD=", 4))
			break ;
	mini->my_env->my_env[i] = mini->path;
}
int	build_cd(t_mini *mini)
{
	char *cd;
	char *cd2;
	int i;
	
	i = 3;
	if(!ft_strcmp(mini->input,"cd"))
	{
		chdir(mini->my_env->home);
		get_pwd(mini);
	}
	else
	{
		cd = ft_strjoin(mini->path,"/");
		cd2 = ft_strjoin(cd,mini->input + 3);
		if (chdir(cd2) < 0)
			ft_printf("Minishell: cd: %s: No such file or directory\n",mini->input + 3);
		get_pwd(mini);
		free(cd2);
		free(cd);
	}
	// pwd_update(mini);
	ft_printf("Mini path = %s\n",mini->path);
	return(1);
}

