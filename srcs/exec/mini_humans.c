#include "../../incs/mini_header.h"

// int	here_doc(t_pipe *pipex)
// {
// 	char	*str;
// 	int		fd[2];
// 	int		i;

// 	pipe(fd);
// 	while (1)
// 	{
// 		i = 0;
// 		str = get_next_line(0);
// 		if (!str || !ft_strncmp(str, pipex->av[2], ft_strlen(pipex->av[2])))
// 		{
// 			free(str);
// 			break ;
// 		}
// 		while (str[i])
// 			write(fd[1], &str[i++], 1);
// 		free(str);
// 	}
// 	close(fd[1]);
// 	return (fd[0]);
// }

void first_child(t_mini *mini,t_cmd cmds)
{

    // ft_printf("First child\n");
    do_redirect(&cmds,mini);
	if(cmds.fdout != -1)
    {
        dup2(cmds.fdout,STDOUT_FILENO);
        close(cmds.fdout);
    }
    else
    {
        // ft_printf("IM PIPING\n");
        dup2(mini->pipex.pipefd[1], STDOUT_FILENO);
        close(mini->pipex.pipefd[1]);
    }
	if(cmds.fdin != -1)
    {
        dup2(cmds.fdin,STDIN_FILENO);
        close(cmds.fdin);
    }
    close(mini->pipex.pipefd[0]);
    cmdexec(mini->env->my_env,cmds,mini);
}

void last_child(t_mini *mini,t_cmd cmds)
{
    
    // dprintf(2,"last child\n");
    do_redirect(&cmds,mini);
	if(cmds.fdout != -1)
    {
		dup2(cmds.fdout,STDOUT_FILENO);
        close(cmds.fdout);
    }
	if(cmds.fdin != -1)
    {
		dup2(cmds.fdin,STDIN_FILENO);
        close(cmds.fdin);
    }
    else
    {
        // ft_printf("IM PIPING\n");
        dup2(mini->save_fd, STDIN_FILENO);
        close(mini->pipex.pipefd[0]);
    }
    close(mini->pipex.pipefd[1]);
    // ft_printf("BEFORE REDIRECT fdin = %d, fdout = %d\n", cmds.fdin, cmds.fdout);
	// ft_printf("fdin = %d\n",cmds.fdin);
    // ft_printf("fdout = %d\n",cmds.fdout);
    cmdexec(mini->env->my_env,cmds,mini);
}

void middle_child(t_mini *mini,t_cmd cmds)
{

    do_redirect(&cmds,mini);
    if(cmds.fdout != -1)
    {
        dup2(cmds.fdout,STDOUT_FILENO);
        close(cmds.fdout);
    }
    else
    {
        dup2(mini->pipex.pipefd[1], STDOUT_FILENO);
        close(mini->pipex.pipefd[1]);
    }
	if(cmds.fdin != -1)
    {
		dup2(cmds.fdin,STDIN_FILENO);
        close(cmds.fdin);
    }
    else
    {
        dup2(mini->save_fd, STDIN_FILENO);
        close(mini->pipex.pipefd[0]);
    }
    cmdexec(mini->env->my_env,cmds,mini);
}

void solo_child(t_mini *mini,t_cmd cmds)
{
    int pid;

    pid = fork();
    if(pid == 0)
    {
        // ft_printf("solo child\n");
        do_redirect(&cmds,mini);
        if (cmds.fdout != -1)
            dup2(cmds.fdout, STDOUT_FILENO);
        if (cmds.fdin != -1)
            dup2(cmds.fdin, STDIN_FILENO);
        // ft_printf("fdin = %d\n",cmds.fdin);
        // ft_printf("fdout = %d\n",cmds.fdout);
        cmdexec(mini->env->my_env,cmds,mini);
    }
}
