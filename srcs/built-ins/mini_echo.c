#include "../../incs/mini_header.h"

static void	redir_echo(t_cmd cmds, int flag, int fd, int t)
{
	int	i;

	i = 0;
	if (t == 0)
		return ;
	else
		dup2(fd, STDOUT_FILENO);
	if (flag != 0)
		i = flag;
	if (cmds.amount == 0)
		ft_printf("\n");
	else
	{
		while (cmds.args[i])
		{
			ft_printf("%s", cmds.args[i]);
			i++;
			if (!cmds.args[i])
				break ;
			ft_printf(" ");
		}
		if (flag == 0)
			ft_printf("\n");
	}
}
static void	normal_echo(t_mini *mini, t_cmd cmds, int flag)
{
	int	i;

	i = 1;
	(void)mini;
	if (flag != 0)
		i = flag;
	if (cmds.amount == 0)
		ft_printf("\n");
	else
	{
		while (cmds.args[i])
		{
			ft_printf("%s", cmds.args[i]);
			i++;
			if (!cmds.args[i])
				break ;
			ft_printf(" ");
		}
		if (flag == 0)
			ft_printf("\n");
	}
}

static int	echo_flag(t_cmd cmds)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (cmds.args[j][i] == 'n')
		i++;
	if (cmds.args[j][i] != '\0')
		return (j);
	j = 1;
	while (cmds.args[j] && cmds.args[j][0] == '-')
	{
		i = 1;
		while (cmds.args[j][i] == 'n')
			i++;
		if (cmds.args[j][i] != '\0')
			return (j);
		j++;
	}
	return (j);
}
int	build_echo(t_mini *mini, t_cmd cmds)
{
	int flag;
	int fd;
	int pid;
	int t;

	flag = 0;
	if (cmds.amount != 1)
		if (cmds.args && cmds.args[0][0] == '-')
			flag = echo_flag(cmds);
	if (cmds.redirections[0].type != T_NULL)
	{
		fd = do_redirect(cmds, &t);
		pid = fork();
		if (pid == 0)
		{
			redir_echo(cmds, flag, fd, t);
			exit_childprocess(mini);
		}
		wait(NULL);
	}
	else
		normal_echo(mini, cmds, flag);
	return (1);
}