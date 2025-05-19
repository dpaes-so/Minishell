
#include "../../incs/mini_header.h"

void	here_doc_expand(char *s, t_mini *mini, int fd[2], bool quotes)
{
	int		i;
	int		j;
	char	*s2;

	i = 0;
	while (s[i])
	{
		j = 0;
		if (s[i] == '$' && quotes == false)
		{
			i++;
			if (s[i] == '?')
			{
				s2 = status_expand(mini);
				while (s2 && s2[j])
					write(fd[1], &s2[j++], 1);
				free(s2);
				return ;
			}
			if (ft_isdigit(s[i]))
			{
				i++;
				continue ;
			}
			s2 = find_in_env(s + i, mini);
			while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
				i++;
			while (s2 && s2[j])
				write(fd[1], &s2[j++], 1);
		}
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
	ft_printf("exec signal on here doc %d\n",mini->execution_signal);
	close(fd[1]);
	return (fd[0]);
}
