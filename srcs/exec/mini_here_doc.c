/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_here_doc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniel <daniel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 20:02:58 by dpaes-so          #+#    #+#             */
/*   Updated: 2025/06/03 15:35:04 by daniel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/mini_header.h"

void	do_here_doc(t_mini *mini, t_tree *ast, int i)
{
	if (!ast)
		return ;
	if (ast->node.pipe == true)
	{
		do_here_doc(mini, ast->left, 0);
		do_here_doc(mini, ast->right, 0);
	}
	else if (ast->node.redir && ast->node.redir[i].type != T_NULL)
	{
		while (ast->node.redir[i].type != T_NULL)
		{
			if (mini->execution_signal != 0)
				break ;
			if (ast->node.redir[i].type == T_HERE_DOC)
				ast->node.here_fd = here_doc(mini->pipex, &ast->node, i, mini);
			i++;
		}
	}
}

static void	here_dollar_handle(int *i, char *s, int fd[2], t_mini *mini)
{
	char	*s2;
	int		j;

	j = 0;
	(*i)++;
	if (s[(*i)] == '?')
	{
		s2 = NULL;
		favila(s2, fd, s, mini);
		return ;
	}
	if (ft_isdigit(s[(*i)]))
	{
		(*i)++;
		return ;
	}
	s2 = find_in_env(s + (*i), mini);
	if (!s2 && mini->f_malloc == 1)
		return (free(s), fmalloc(mini, "here dolllar ", 100));
	while (s[(*i)] && (ft_isalnum(s[(*i)]) || s[(*i)] == '_'))
		(*i)++;
	while (s2 && s2[j])
		write(fd[1], &s2[j++], 1);
}

void	here_doc_expand(char *s, t_mini *mini, int fd[2], bool quotes)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '$' && quotes == false)
			here_dollar_handle(&i, s, fd, mini);
		else
			write(fd[1], &s[i++], 1);
	}
}

void	here_loop(int j, t_cmd *cmds, int fd[2], t_mini *mini)
{
	char	*str;

	while (1)
	{
		str = readline("> ");
		if (!str || !ft_strcmp(str, cmds->redir[j].value))
		{
			free(str);
			break ;
		}
		here_doc_expand(str, mini, fd, cmds->redir[j].in_quotes);
		write(fd[1], "\n", 1);
		free(str);
	}
}

int	here_doc(t_pipe pipex, t_cmd *cmds, int j, t_mini *mini)
{
	int	fd[2];
	int	pid;

	(void)pipex;
	pipe(fd);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		signals(3);
		here_loop(j, cmds, fd, mini);
		exit_childprocess(mini, 0);
	}
	else
		wait(&mini->execution_signal);
	if (WIFEXITED(mini->execution_signal))
		mini->execution_signal = WEXITSTATUS(mini->execution_signal);
	if (mini->execution_signal == 100)
		fmalloc(mini, "here doc", 1);
	close(fd[1]);
	return (fd[0]);
}
