#include "../../incs/mini_header.h"


void first_child(t_mini *mini,t_cmd cmds)
{
    int fd;
    int t;

    ft_printf("First child\n");
    fd = do_redirect(&cmds,&t);
	if(fd != 1 && cmds.fdout != 0)
		dup2(cmds.fdout,STDOUT_FILENO);
	if(fd != 1 && cmds.fdin!= 0)
		dup2(cmds.fdin,STDIN_FILENO);
    
	ft_printf("fdin = %d\n",cmds.fdin);
    ft_printf("fdout = %d\n",cmds.fdout);
    cmdexec(mini->env->my_env,cmds,mini);
}

void last_child(t_mini *mini,t_cmd cmds)
{
    int fd;
    int t;
    
    ft_printf("last child\n");
    fd = do_redirect(&cmds,&t);
	if(fd != 1 && cmds.fdout != 0)
		dup2(cmds.fdout,STDOUT_FILENO);
	if(fd != 1 && cmds.fdin != 0)
		dup2(cmds.fdin,STDIN_FILENO);
	ft_printf("fdin = %d\n",cmds.fdin);
    ft_printf("fdout = %d\n",cmds.fdout);
    cmdexec(mini->env->my_env,cmds,mini);
}

void middle_child(t_mini *mini,t_cmd cmds)
{
     int fd;
    int t;

    ft_printf("middle child\n");
    fd = do_redirect(&cmds,&t);
	if(fd != 1 && cmds.fdout!= 0)
		dup2(cmds.fdout,STDOUT_FILENO);
	if(fd != 1 && cmds.fdin != 0)
		dup2(cmds.fdin,STDIN_FILENO);
	ft_printf("fdin = %d\n",cmds.fdin);
    ft_printf("fdout = %d\n",cmds.fdout);
    cmdexec(mini->env->my_env,cmds,mini);
}

void solo_child(t_mini *mini,t_cmd cmds)
{
    int fd;
    int t;
    int pid;

    pid = fork();
    if(pid == 0)
    {
        ft_printf("solo child\n");
        fd = do_redirect(&cmds,&t);
        if(fd != 1 && cmds.fdout!= 0)
            dup2(cmds.fdout,STDOUT_FILENO);
        if(fd != 1 && cmds.fdin != 0)
            dup2(cmds.fdin,STDIN_FILENO);
        ft_printf("fdin = %d\n",cmds.fdin);
        ft_printf("fdout = %d\n",cmds.fdout);
        cmdexec(mini->env->my_env,cmds,mini);
    }
}
