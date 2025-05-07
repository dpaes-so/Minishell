/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:27:27 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/05/05 15:03:40 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

int	check_built_in(t_mini *mini, t_cmd cmds)
{
	if (!cmds.cmd)
		return (0);
	if (ft_strncmp(cmds.cmd, "echo", 4) == 0)
		return (build_echo(mini, cmds));
	if (ft_strcmp(cmds.cmd, "pwd") == 0)
		return (build_pwd(mini, cmds));
	if (ft_strcmp(cmds.cmd, "env") == 0)
		return (build_env(mini, cmds));
	if (ft_strncmp(cmds.cmd, "cd", 2) == 0)
		return (build_cd(mini, cmds));
	if (ft_strncmp(cmds.cmd, "export", 6) == 0)
		return (build_export(mini, cmds));
	if (ft_strncmp(cmds.cmd, "unset", 5) == 0)
		return (build_unset(mini, cmds));
	if (ft_strcmp(cmds.cmd, "exit") == 0)
		build_exit(mini, cmds);
	return (0);
}

void	cmdexec(char *envp[], t_cmd cmds, t_mini *mini)
{
	int		i;
	char	*exec;
	int		flag;

	flag = 0;
	i = 0;
	if (check_built_in(mini, cmds))
		exit_childprocess(mini);
	while (flag == 0 && cmds.cmd)
	{
		if (i > 0)
			free(exec);
		if (mini->pipex.path != NULL && mini->pipex.path[i] && (access(cmds.cmd,
					F_OK) < 0))
			exec = ft_strjoin(mini->pipex.path[i], cmds.cmd);
		else
		{
			exec = ft_strdup(cmds.cmd);
			flag = 1;
		}
		master_close();
		execve(exec, cmds.args, envp);
		i++;
	}
	cmd_exit(exec, mini);
}

void	which_child(t_mini *mini, t_cmd cmds)
{
	signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
	mini->pipex.pid1 = fork();
	if (mini->pipex.pid1 == 0)
	{
		choose_signal(2);
		if (mini->pipex.cmd == 0)
			first_child(mini, cmds);
		else if (mini->pipex.cmd == mini->cmd_amount - 1)
			last_child(mini, cmds);
		else
			middle_child(mini, cmds);
	}
	else
	{
		close(mini->pipex.pipefd[1]);
		mini->save_fd = dup(mini->pipex.pipefd[0]);
		close(mini->pipex.pipefd[0]);
	}
	mini->pipex.cmd++;
}

void	execute(t_mini *mini, t_tree *ast, int f)
{
	if (f == 0)
	{
		if (check_built_in(mini, ast->node))
			return ;
		else
			solo_child(mini, ast->node);
	}
	else if (pipe(mini->pipex.pipefd) == 0)
		which_child(mini, ast->node);
	else
	{
		ft_putstr_fd("Error, Pipe faield", 2);
		exit(1);
	}
}

void	run_tree(t_mini *mini, t_tree *ast, int f)
{
	if (ast->node.pipe == true)
	{
		run_tree(mini, ast->left, 1);
		run_tree(mini, ast->right, 1);
	}
	else
		execute(mini, ast, f);
}
