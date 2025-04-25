#include "../../incs/mini_header.h"

int	build_unset(t_mini *mini,t_cmd cmds)
{
	int	i;
	int cmd_n;

	cmd_n = -1;
	do_redirect(cmds);
	if (!cmds.args[0])
		return (0);
	while(cmds.args[++cmd_n])
	{
		i = -1;
		printf("IMA TRY TO UNSET THIS  \n%s\n", cmds.args[cmd_n]);
		while (mini->env->my_env[++i])
			if (ft_strncmp(mini->env->my_env[i], cmds.args[cmd_n],
					ft_strlen(cmds.args[cmd_n])) == 0
				&& mini->env->my_env[i][ft_strlen(cmds.args[cmd_n])] == '=')
				break ;
		printf("UNSETTING SOMETHING BITCH\n%s\n", mini->env->my_env[i]);
		while (mini->env->my_env[i] != NULL)
		{
			free(mini->env->my_env[i]);
			mini->env->my_env[i] = ft_strdup(mini->env->my_env[i + 1]);
			i++;
		}
	}
	return (1);
}