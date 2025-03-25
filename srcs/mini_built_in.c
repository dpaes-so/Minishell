/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:42:40 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/03/25 19:01:06 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../incs/mini_header.h"

void	build_exit(t_mini *mini)
{
	free(mini->input);
	if (mini->path)
		free(mini->path);
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

// build_env();
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
void	build_cd(t_mini *mini)
{
	char *cd;
	char *cd2;
	if(!ft_strcmp(mini->input,"cd"))
		chdir("/home/dpaes-so");
	else
	{
		cd = ft_strjoin(mini->path,"/");
		cd2 = ft_strjoin(cd,mini->input + 3);
		chdir(cd2);
		get_pwd(mini);
		free(cd2);
		free(cd);
	}
}
