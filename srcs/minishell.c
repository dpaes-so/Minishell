/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:46:22 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/04/29 20:48:02 by dgarcez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/mini_header.h"

void	handle_signal(int signal)
{
	if (signal == SIGINT)
	{
		rl_on_new_line();
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_redisplay();
	}
	return ;
}

void	sig_init(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_signal;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1)
		return (perror("Failed sigaction"));
}
void	my_env_start(t_mini *mini, char **ev)
{
	int	i;
	int	j;
	int	k;

	i = -1;
	j = 0;
	k = 0;
	mini->env = malloc(sizeof(t_env));
	if (mini->env == NULL)
		return ;
	while (ev[k])
		k++;
	mini->env->my_env = (char **)ft_calloc(k + 1, sizeof(char *));
	if (mini->env->my_env == NULL)
		return ;
	while (ev[j])
	{
		mini->env->my_env[j] = ft_strdup(ev[j]);
		if (mini->env->my_env[j] == NULL)
			return ;
		j++;
	}
	while (ev[++i])
		if (ft_strnstr(ev[i], "HOME=", 5))
			break ;
	mini->env->home = NULL;
	if(ev[i])
		mini->env->home = ft_strdup(ev[i] + 5);
	if (mini->env->home == NULL)
		return ;
}

int	check_built_in(t_mini *mini, t_cmd cmds)
{
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

void	cmd_exit(char *exec, char **argument_list,t_mini *mini,char **abc,char **path)
{
	// (void)mini;
	if (access(exec, F_OK) < 0)
	{
		ft_putstr_fd("Pipex: Command not found\n", 2);
		// dprintf(2,"exec = %s\n",exec);
		freetrix(argument_list);
		freetrix(mini->env->my_env);
		free(mini->pwd);
		if(mini->env->home != NULL)
			free(mini->env->home);
		freetrix(path);
		free_tree(mini->ast);
		freetrix(abc);
		if (exec)
			free(exec);
		exit(127);
	}
	if (access(exec, X_OK) < 0)
	{
		ft_putstr_fd("Permission denied\n", 2);
		freetrix(argument_list);
		freetrix(mini->env->my_env);
		free(mini->pwd);
		if(mini->env->home != NULL)
			free(mini->env->home);
		// free_tree(mini->ast);
		// freetrix(mini->pipex.path);
		freetrix(abc);
		if (exec)
			free(exec);
		exit(126);
	}
}

void	cmdexec(t_pipe *pipe, char *envp[],t_cmd cmds,t_mini *mini)
{
	int		i;
	char	*exec;
	int		flag;
	char **abc;
	char  **path;

	flag = 0;
	i = 0;
	pipe->pid1 = fork();
	if(pipe->pid1 == 0)
	{
		path = path_finder(mini->env->my_env,mini->pipex);
		abc = ft_split(cmds.cmd,' ');
		while (flag == 0 && cmds.cmd)
		{
			if (i > 0)
				free(exec);
			if (pipe->path != NULL && pipe->path[i] && (access(cmds.cmd, F_OK) < 0 ))
				exec = ft_strjoin(pipe->path[i], cmds.cmd);
			else
			{
				exec = ft_strdup(cmds.cmd);
				flag = 1;
			}
			master_close();
			if(cmds.amount == 0)
				execve(exec,abc,envp);
			else
				execve(exec,cmds.args,envp);
			i++;
		}
		cmd_exit(exec,cmds.args,mini,abc,path);
	}
	(void)path;
}

// void	cmdexec(t_pipe *pipe, char *envp[],t_cmd cmds)
// {
// 	(void)pipe;
// 	// (void)cmds;
// 	// (void)
// 	char **abc = ft_split(cmds.cmd,' ');
// 	master_close();
// 	if(cmds.amount == 0)
// 		execve("/usr/bin/ls",abc,envp);
// 	else
// 		execve("/usr/bin/ls",cmds.args,envp);
// }
void which_child(t_mini *mini,t_cmd cmds)
{
	// int i;

	// // i =0;
	// check_built_in(mini,cmds);
	if(check_built_in(mini,cmds))
		return;
	else
		cmdexec(&mini->pipex,mini->env->my_env,cmds,mini);
	// while(mini->pipex.path[i])
	// {
	// 	ft_printf("path = %s\n",mini->pipex.path[i]);
	// 	i++;
	// }
}

void	execute(t_mini *mini, t_tree *ast)
{
	if(pipe(mini->pipex.pipefd) == 0)
    	which_child(mini,ast->node);
    else
    {
        ft_putstr_fd("Error, Pipe faield",2);
        exit(1);
    }
}

void	run_tree(t_mini *mini, t_tree *ast)
{
	if (ast->node.pipe == true)
	{
		run_tree(mini, ast->left);
		run_tree(mini, ast->right);
	}
	else
	{
		execute(mini, ast);
		// printf("commad = %s\n",ast->node.cmd);
	}
}
int	main(int ac, char **av, char **ev)
{

	t_mini mini;
	t_tree *ast;
	char*input;

	(void)ac;
	(void)av;
	// sig_init();
	ft_bzero(&mini, sizeof(t_mini));
	my_env_start(&mini, ev);
	get_pwd(&mini);
	while (1)
	{
		input = readline("minishell > ");
		add_history(input);
		mini.ast = parser(input,mini);
		ast = mini.ast;
		if (mini.ast == NULL)
			continue ;
		tree_apply_infix(mini.ast, 0, "root");
		run_tree(&mini, ast);
		wait(NULL);
		free_tree(mini.ast);
		free(input);
	}
}
