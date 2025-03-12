/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:42:40 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/03/12 16:58:04 by dpaes-so         ###   ########.fr       */
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