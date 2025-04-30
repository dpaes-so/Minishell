#include "../../incs/mini_header.h"

static void check_exit_code(t_mini *mini,t_cmd cmds)
{
    int j;

	j = -1;
    while(cmds.args[0][++j])
	{
		if(!(cmds.args[0][j] >= '0' && cmds.args[0][j] <= '9'))
		{
			ft_putstr_fd("Minishell: exit: Numerical input required\n",2);
			mini->pipex.status =255;
            return ;
		}
	}
}
int	build_exit(t_mini *mini,t_cmd cmds)
{
	int t;
	
    do_redirect(cmds,&t);
	mini->pipex.status = 0;
	if(cmds.amount  > 1)
		return(ft_putstr_fd("Minishell: exit: too many argumetns\n",2),1);
	if(cmds.amount != 0)
		mini->pipex.status = ft_atoi(cmds.args[0]);
	if(cmds.args[0])
		check_exit_code(mini,cmds);
	free(mini->pwd);
	if(mini->env->home != NULL)
		free(mini->env->home);
	freetrix(mini->env->my_env);
	free(mini->env);
	free_tree(mini->ast);
	// freetrix(mini->pipex.path);
	clear_history();
	master_close();
	exit(mini->pipex.status);
	return(1);
}
