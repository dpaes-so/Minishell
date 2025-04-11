/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:42:40 by dgarcez-          #+#    #+#             */
/*   Updated: 2025/04/11 18:35:37 by dpaes-so         ###   ########.fr       */
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
		if (ft_strncmp(mini->env->my_env[i], mini->input + 6,
				ft_strlen(mini->input + 6)) == 0
			&& mini->env->my_env[i][ft_strlen(mini->input + 6)] == '=')
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

int	print_env_ex(t_mini *mini)
{
	int	i;

	i = -1;
	while (mini->env->my_env[++i])
		ft_printf("declare -x %s\n", mini->env->my_env[i]);
	return(1);
}

int	check_valid_variable_name(char *s)
{
	int	i;

	i = 0;
	if (s[0] == '=')
		return (0);
	if (!((s[0] >= 'a' && s[0] <= 'z') || (s[0] >= 'A' && s[0] <= 'Z')
			|| (s[0] == '_')))
		return (0);
	while (s[i] && s[i] != '=')
	{
		if (!((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')
				|| (s[i] >= '0' && s[i] <= '9') || (s[i] == '_')))
			return (0);
		i++;
	}
	return (i);
}

char	*ft_strchr(const char *str, int c)
{
	int		i;
	char	*s;

	i = 0;
	s = (char *)str;
	while (s[i])
	{
		if (str[i] == (char)c)
			return (s + i);
		i++;
	}
	return (NULL);
}
void	*make_export(t_mini *mini, char *arg)
{
	char	**new_env;
	int		size;
	int		break_point;
	int		i;

	size = 0;
	i = -1;
	break_point = 0;
	while (mini->env->my_env[size])
		size++;
	new_env = malloc(sizeof(char *) * (size + 2));
	if (!new_env)
		return (NULL);
	while (mini->env->my_env[break_point])
	{
		if (!ft_strncmp(mini->env->my_env[break_point], arg, ft_strchr(arg, '=')
				- arg))
		{
			free(mini->env->my_env[break_point]);
			mini->env->my_env[break_point] = ft_strdup(arg);
			return (NULL);
		}
		break_point++;
	}
	while (++i < size)
		new_env[i] = mini->env->my_env[i];
	new_env[i] = arg;
	new_env[++i] = NULL;
	free(mini->env->my_env);
	mini->env->my_env = new_env;
	return (NULL);
}
int	build_export(t_mini *mini)
{
	char	*arg;
	char	**split;
	int		i;
	int		split_index;

	if(!ft_strcmp(mini->input,"export") || !ft_strcmp(mini->input,"export "))//change to no arg after parser
		return(print_env_ex(mini));
	split_index = 0;
	split = ft_split(mini->input + 7, ' ');
	while (split[split_index])
	{
		arg = split[split_index];
		i = check_valid_variable_name(arg);
		ft_printf("!%s!\n",arg);
		if (!i)
		{
			ft_printf("Minishell: '%s' not a valid identifier\n", arg);
			split_index++;
			continue ;
		}
		make_export(mini, arg);
		split++;
	}
	freetrix(split);
	return (1);
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
	char	*arg;

	arg = mini->input + 3;
	if (!ft_strcmp(mini->input, "cd"))
	{
		chdir(mini->env->home);
		get_pwd(mini);
	}
	else
	{
		if (arg[0] == '/')
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
			ft_printf("Minishell: cd: %s: No such file or directory\n",
				mini->input + 3);
		else
			get_pwd(mini);
		free(cd2);
	}
	pwd_update(mini);
	ft_printf("Mini path = %s\n", mini->pwd);
	return (1);
}
