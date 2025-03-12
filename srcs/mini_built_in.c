/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:42:40 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/03/12 18:36:19 by dgarcez-         ###   ########.fr       */
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
		printf("%s", mini->input + 8);
	else
		printf("%s\n", mini->input + 5);
}

// build_env();
// build_unset();
// build_export();
// build_pwd();

void	build_cd(t_mini *mini)
{
	char	*temp;
	char	*cdw;

	temp = NULL;
	cdw = getcwd(temp, 100);
	free(mini->path);
	mini->path = ft_strjoin("minishell >", cdw);
	free(temp);
	free(cdw);
}