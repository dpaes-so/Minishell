#include "../../incs/mini_header.h"

int	check_built_in(t_mini *mini, t_cmd cmds)
{
	// if (!cmds.cmd)
	// 	return (0);
	printf("!%s!\n",cmds.cmd);
	if (ft_strncmp(cmds.cmd, "echo",4) == 0)
		return(build_echo(mini,cmds));
	if (ft_strcmp(cmds.cmd, "pwd") == 0)
		return (build_pwd(mini,cmds));
	if (ft_strcmp(cmds.cmd, "env") == 0)
		return (build_env(mini,cmds));
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

void	cmd_exit(char *exec,t_mini *mini)
{
	if (access(exec, F_OK) < 0)
	{
		ft_putstr_fd("Pipex: Command not found\n", 2);
		free(mini->pwd);
		free(mini->env->home);
		free(mini->pipex.path);
		freetrix(mini->env->my_env);
		free(mini->env);
		free_tree(mini->ast);
		if (exec)
			free(exec);
		exit(127);
	}
	if (access(exec, X_OK) < 0)
	{
		ft_putstr_fd("Permission  2 denied\n", 2);
		free(mini->pwd);
		free(mini->env->home);
		freetrix(mini->pipex.path);
		freetrix(mini->env->my_env);
		free(mini->env);
		free_tree(mini->ast);
		if (exec)
			free(exec);
		exit(126);
	}
}

void	cmdexec(char *envp[],t_cmd cmds,t_mini *mini)
{
	int		i;
	char	*exec;
	int		flag;

	flag = 0;
	i = 0;
	while (flag == 0 && cmds.cmd)
	{
		if (i > 0)
			free(exec);
		if (mini->pipex.path != NULL && mini->pipex.path[i] && (access(cmds.cmd, F_OK) < 0 ))
			exec = ft_strjoin(mini->pipex.path[i], cmds.cmd);
		else
		{
			exec = ft_strdup(cmds.cmd);
			flag = 1;
		}
		master_close();
		ft_printf("exec = %s\n",exec);
		execve(exec, cmds.args, envp);
		i++;
	}
	cmd_exit(exec,mini);
}

void which_child(t_mini *mini,t_cmd cmds)
{
	static int cmd_n;

	mini->pipex.pid1 = fork();
	if(mini->pipex.pid1 == 0)
	{
		if (cmd_n == 0)
			first_child(mini,cmds);
		else if (cmd_n == mini->cmd_amount - 1)
			last_child(mini,cmds);
		else
			middle_child(mini,cmds);
	}
	cmd_n++;
	// if(mini->pipex.pid1 == 0)
	// 	cmdexec(&mini->pipex,mini->env->my_env,cmds,mini);
}

void	execute(t_mini *mini, t_tree *ast,int f)
{
	if(f == 0)
	{
		ft_printf("AYOOO\n");
		if(check_built_in(mini,ast->node))
			return ;
		else
			solo_child(mini,ast->node);
	}
	else if(pipe(mini->pipex.pipefd) == 0)
    	which_child(mini,ast->node);
    else
    {
        ft_putstr_fd("Error, Pipe faield",2);
        exit(1);
    }
}