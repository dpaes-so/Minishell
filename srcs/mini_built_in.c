/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:42:40 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/04/09 19:45:02 by dpaes-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/mini_header.h"

void	build_exit(t_mini *mini)
{
	free(mini->input);
	if (mini->pwd)
		free(mini->pwd);
	free(mini->env->home);
	freetrix(mini->env->my_env);
	free(mini->env);
	clear_history();
	exit(0);
}

void	build_echo(t_mini *mini)
{
	if (ft_strncmp(mini->input + 5, "-n ", 3) == 0)
		ft_printf("%s", mini->input + 8);
	else
		ft_printf("%s\n", mini->input + 5);
}

int	build_env(t_mini *mini)
{
	int	i;

	i = -1;
	while (mini->env->my_env[++i])
		ft_printf("%s\n", mini->env->my_env[i]);
	return (1);
}
int	build_unset(t_mini *mini)
{
	int	i;

	i = -1;
	while (mini->env->my_env[++i])
		if (ft_strncmp(mini->env->my_env[i], mini->input + 6, ft_strlen(mini->input + 6)) == 0 && mini->env->my_env[i][ft_strlen(mini->input + 6)] == '='
)
			break ;
	
	printf("UNSETTING SOMETHING BITCH\n%s\n", mini->env->my_env[i]);
	while (mini->env->my_env[i] != NULL && mini->env->my_env[i + 1])
	{
		free(mini->env->my_env[i]);
		mini->env->my_env[i] = ft_strdup(mini->env->my_env[i + 1]);
		i++;
	}
	return (1);
}

void print_env_ex(t_mini *mini)
{
	int	i;

	i = -1;
	while (mini->env->my_env[++i])
		ft_printf("declare -x %s\n", mini->env->my_env[i]);
}

int check_valid_variable_name(char *s)
{
	int i;

	i = 0;
	if(s[0] == '=')
		return(0);
	if(!((s[0] >= 'a' && s[0] <= 'z') || (s[0] >= 'A' && s[0] <= 'Z') || (s[0] == '_')))
		return(0);
	while(s[i] && s[i] != '=')
	{
		if(!((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= '0' && s[i] <= '1') ||  (s[i] == '_')))
			return(0);
		i++;
	}
	// if(s[i] != '=')
	// 	GO TO FUNC TO SEE IF IT EXIS AND UPD TO NULL OR JUST CREAT
	return(i);
}
void make_export(t_mini *mini,char *arg)
{

}
int build_export(t_mini *mini)
{
	// ft_printf("!%s!\n",mini->input);
	char *arg;
	char *split;
	int i;
	int split_index;
	// if(!ft_strcmp(mini->input,"export"))//change to no arg after parser
	// 	print_env_ex(mini);
	split_index = 0;
	split = ft_split(mini->input + 7,' ');
	while (split[split_index])
	{
		arg =split[split_index];
		i = check_valid_variable_name(arg);
        if (!i)
        {
            ft_printf("Minishell: '%s' not a valid identifier\n", arg);
            split_index++;
            continue;
        }
		if(arg[i] != '=')
			ft_printf("ADD HAS AN NULL VAL")
		ft_printf("OK ALL GOOD GOTTA IMPLEMENT THE REST THO   %d\n",i);
	}
	return(1);
}
int	build_pwd(t_mini *mini)
{
	ft_printf("%s\n", mini->pwd);
	return (1);
}

void	get_pwd(t_mini *mini)
{
	char	*cdw;

	cdw = getcwd(NULL, 100);
	free(mini->pwd);
	mini->pwd = ft_strjoin(cdw, "");
	free(cdw);
}
void	pwd_update(t_mini *mini)
{
	int	i;

	i = -1;
	while (mini->env->my_env[++i])
		if (ft_strnstr(mini->env->my_env[i], "PWD=", 4))
			break ;
	free(mini->env->my_env[i]);
	mini->env->my_env[i] = ft_strjoin("PWD=", mini->pwd);
}
int	build_cd(t_mini *mini)
{
	char	*cd;
	char	*cd2;
	char *arg;

	arg = mini->input + 3;
	if (!ft_strcmp(mini->input, "cd"))
	{
		chdir(mini->env->home);
		get_pwd(mini);
	}
	else
	{
		if(arg[0] == '/')
		{
			cd2 = ft_strdup(mini->input + 3);
			printf("ABSOLUTE PATH DETED\n");
		}
		else
		{
			cd = ft_strjoin(mini->pwd, "/");
			cd2 = ft_strjoin(cd, mini->input + 3);
			free(cd);
		}
		if (chdir(cd2) < 0)
			ft_printf("Minishell: cd: %s: No such file or directory\n",mini->input + 3);
		else 
			get_pwd(mini);
		free(cd2);
	}
	pwd_update(mini);
	ft_printf("Mini path = %s\n", mini->pwd);
	return (1);
}
