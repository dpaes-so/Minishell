/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:42:40 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/03/12 15:44:45 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	build_exit(char	**input);
{
	free(*input);
	clear_history();
	exit(0);
}

		build_env();		
		build_unset();
		build_export();
		build_pwd();
		build_cd();
		build_echo();