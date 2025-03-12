/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:42:40 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/03/12 16:09:25 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/mini_header.h"

void	build_exit(t_mini *mini)
{
	free(mini->input);
	if(mini->path)
		free(mini->path);
	clear_history();
	exit(0);
}

		// build_env();		
		// build_unset();
		// build_export();
		// build_pwd();
		// build_cd();
		// build_echo();