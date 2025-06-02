/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:27:27 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/06/02 14:44:16 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

int	check_built_in(t_mini *mini, t_cmd cmds)
{
	if (!cmds.cmd)
		return (0);
	if (ft_strcmp(cmds.cmd, "echo") == 0)
		return (build_echo(mini, cmds));
	if (ft_strcmp(cmds.cmd, "pwd") == 0)
		return (build_pwd(mini, cmds));
	if (ft_strcmp(cmds.cmd, "env") == 0)
		return (build_env(mini, cmds));
	if (ft_strcmp(cmds.cmd, "cd") == 0)
		return (build_cd(mini, cmds));
	if (ft_strcmp(cmds.cmd, "export") == 0)
		return (build_export(mini, cmds));
	if (ft_strcmp(cmds.cmd, "unset") == 0)
		return (build_unset(mini, cmds));
	if (ft_strcmp(cmds.cmd, "exit") == 0)
		return (build_exit(mini, cmds));
	return (0);
}

void	cmdexec(char *envp[], t_cmd cmds, t_mini *mini)
{
	int		i;
	char	*exec;
	int		flag;

	flag = 0;
	i = -1;
	exec = NULL;
	if (check_built_in(mini, cmds))
		exit_childprocess(mini, 0);
	while (flag == 0 && cmds.cmd && ++i > -1)
	{
		free(exec);
		exec = get_path(cmds, mini, i, &flag);
		if (!exec)
		{
			while (mini->pipex.path[++i])
				free(mini->pipex.path[i]);
			free(mini->pipex.path);
			mini->pipex.path = NULL;
			fmalloc(mini, "cmdexec", 100);
		}
		master_close();
		execve(exec, cmds.args, envp);
	}
	cmd_exit(exec, mini, cmds.cmd);
}

void	which_child(t_mini *mini, t_cmd cmds)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	mini->pipex.pid1 = fork();
	if (mini->pipex.pid1 == 0)
	{
		mem_save(mini);
		signals(2);
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
		if (mini->save_fd != -1)
			close(mini->save_fd);
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
	if (mini->f_malloc == 1)
		fmalloc(mini, "execute", 2);
}

void	run_tree(t_mini *mini, t_tree *ast, int f)
{
	if (!ast)
		return ;
	if (ast->node.pipe == true)
	{
		run_tree(mini, ast->left, 1);
		run_tree(mini, ast->right, 1);
	}
	else
		execute(mini, ast, f);
}
