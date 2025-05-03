#include "../../incs/mini_header.h"


void first_child(t_mini *mini,t_cmd cmds)
{
    int t;

    ft_printf("First child\n");
    do_redirect(&cmds,&t);
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
    int t;
    
    dprintf(2,"last child\n");
    do_redirect(&cmds,&t);
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
    int t;

    do_redirect(&cmds,&t);
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
    int t;
    int pid;

    pid = fork();
    if(pid == 0)
    {
        ft_printf("solo child\n");
        do_redirect(&cmds,&t);
        if (cmds.fdout != -1)
            dup2(cmds.fdout, STDOUT_FILENO);
        if (cmds.fdin != -1)
            dup2(cmds.fdin, STDIN_FILENO);
        ft_printf("fdin = %d\n",cmds.fdin);
        ft_printf("fdout = %d\n",cmds.fdout);
        cmdexec(mini->env->my_env,cmds,mini);
    }
}
