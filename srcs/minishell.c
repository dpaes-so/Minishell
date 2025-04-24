/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:46:22 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/04/23 17:34:39 by dgarcez-         ###   ########.fr       */
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
	while (ev[k])
		k++;
	mini->env->my_env = (char **)ft_calloc(k + 1, sizeof(char *));
	while (ev[j])
	{
		mini->env->my_env[j] = ft_strdup(ev[j]);
		j++;
	}
	while (ev[++i])
		if (ft_strnstr(ev[i], "HOME=", 5))
			break ;
	mini->env->home = ft_strdup(ev[i] + 5);
}

int	check_built_in(t_mini *mini,t_cmd cmds)
{
	// if (!cmds.cmd)
	// 	return (0);
	printf("!%s!\n",cmds.cmd);
	// if (ft_strncmp(cmds.cmd, "echo ", 5) == 0)
	// 	// build_echo(mini);
	if (ft_strcmp(cmds.cmd, "pwd") == 0)
		return (build_pwd(mini));
	if (ft_strcmp(cmds.cmd, "env") == 0)
		return (build_env(mini));
	if (ft_strncmp(cmds.cmd, "cd", 2) == 0)
		return (build_cd(mini,cmds));
	if (ft_strncmp(cmds.cmd, "export", 6) == 0)
		return (build_export(mini,cmds));
	if (ft_strncmp(cmds.cmd, "unset", 5) == 0)
		return (build_unset(mini,cmds));
	if (ft_strcmp(cmds.cmd, "exit") == 0)
		build_exit(mini,cmds);
	return (0);
}

void which_child(t_mini *mini,t_tree *ast)
{
	check_built_in(mini,ast->node);
}

void execute(t_mini *mini,t_tree *ast)
{
	if(pipe(mini->pipex.pipefd) == 0)
    	which_child(mini,ast);
    else
    {
        ft_putstr_fd("Error, Pipe faield",2);
        exit(1);
    }
}

void run_tree(t_mini *mini,t_tree *ast)
{
	if (ast->node.pipe == true)
	{
		run_tree(mini,ast->left);
		run_tree(mini,ast->right);
	}
	else
	{
		execute(mini,ast);
		// printf("commad = %s\n",ast->node.cmd);
	}
}
int	main(int ac, char **av,char **ev)
{
	char	*input;

	t_mini mini;
	t_tree *ast;
	(void)ac;
	(void)av;
	ft_bzero(&mini, sizeof(t_mini));
	my_env_start(&mini,ev);
	get_pwd(&mini);
	while (1)
	{

		input = readline("minishell > ");
		add_history(input);
		// printf("str = %s\n", input);
		mini.ast = parser(input);
		ast = mini.ast;
		if (mini.ast == NULL)
			continue;
		tree_apply_infix(mini.ast, 0, "root");
		run_tree(&mini,ast);
		free_tree(mini.ast);
		free(input);
	}
}
