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

void	run_tree(t_mini *mini, t_tree *ast,int f)
{
	if (ast->node.pipe == true)
	{
		run_tree(mini, ast->left,1);
		run_tree(mini, ast->right,1);
	}
	else
	{
		execute(mini, ast,f);
		// printf("commad = %s\n",ast->node.cmd);
	}
}

void wait_child(t_mini *mini)
{
	int	i;
	int	status;

	status = 0;
	i = 0;
	while (i < mini->cmd_amount)
	{
		wait(&status);
		if (WIFEXITED(status))
			mini->pipex.status = WEXITSTATUS(status);
		i++;
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
	mini.pipex.path = path_finder(ev);
	while (1)
	{
		mini.save_fd = -1;
		mini.cmd_amount = 0;
		input = readline("minishell > ");
		add_history(input);
		mini.ast = parser(input, &mini);
		ast = mini.ast;
		if (mini.ast == NULL)
			continue ;
		// printf("amount of cmds = %d", mini.cmd_amount);
		tree_apply_infix(mini.ast, 0, "root");
		mini.pipex.cmd = 0;
		run_tree(&mini, ast,0);
		master_close();
		wait_child(&mini);
		if(mini.ast)
			free_tree(mini.ast);
		free(input);
	}
}
