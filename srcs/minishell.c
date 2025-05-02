/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:46:22 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/05/02 17:24:40 by dgarcez-         ###   ########.fr       */
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
	// if (!cmds.cmd)
	// 	return (0);
	// printf("!%s!\n",cmds.cmd);
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


void which_child(t_mini *mini,t_cmd cmds)
{
	// mini->pipex.pid1 = fork();
    // if(mini->pipex.pid1 < 0)
    //     exit(1);
    // if(mini->pipex.pid1 == 0)
    // {
    //     close(mini->pipex.pipefd[0]);
	// 	if (mini->pipex.cmd == !(ft_strncmp("here_doc", cmds.cmd, -1)) + 2)
	// 		first_child(mini->pipex,mini->env->my_env);
	// 	else if (mini->pipex.cmd == mini->pipex->ac - 2)
	// 		last_child(mini->pipex,mini->env->my_env);
	// 	else
	// 		middle_child(mini->pipex,mini->env->my_env);
    // }
    // else
    // {
    //     if (mini->pipex.cmd == !(ft_strncmp("here_doc", mini->pipex.av[1], -1)) + 2)
    //         close(mini->pipex.infile_fd);
    //     else if (mini->pipex.cmd == mini->pipex.ac -2)
    //         close(mini->pipex.outfile_fd);
    //     close(mini->pipex.pipefd[1]);
    //     dup2(mini->pipex.pipefd[0],STDIN_FILENO);
    //     close(mini->pipex.pipefd[0]);
    // }
	check_built_in(mini,cmds);
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
	sig_init();
	ft_bzero(&mini, sizeof(t_mini));
	my_env_start(&mini, ev);
	get_pwd(&mini);
	while (1)
	{
		mini.cmd_amount = 0;
		input = readline("minishell > ");
		add_history(input);
		mini.ast = parser(input, &mini);
		ast = mini.ast;
		if (mini.ast == NULL)
			continue ;
		printf("amount of cmds = %d", mini.cmd_amount);
		tree_apply_infix(mini.ast, 0, "root");
		run_tree(&mini, ast);
		free_tree(mini.ast);
		free(input);
	}
}
