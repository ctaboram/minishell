/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:42:44 by ctaboada          #+#    #+#             */
/*   Updated: 2025/11/13 09:42:32 by nacuna-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <sys/stat.h>

static void	ft_remove_last_dir(char *path)
{
	int	i;

	i = ft_strlen(path) - 1;
	while (i > 0 && path[i] == '/')
		i--;
	while (i > 0 && path[i] != '/')
		i--;
	if (i == 0)
		path[1] = '\0';
	else
		path[i] = '\0';
}

static void	ft_strjoin_path(const char *base, const char *rel, char *result)
{
	ft_strlcpy(result, base, MAX_PROMPT_SIZE);
	if (result[ft_strlen(result) - 1] != '/')
		ft_strlcat(result, "/", MAX_PROMPT_SIZE);
	ft_strlcat(result, rel, MAX_PROMPT_SIZE);
}

static int	find_valid_parent(char *oldpwd, char *newpwd)
{
	struct stat	st;
	size_t		prev_len;

	ft_strlcpy(newpwd, oldpwd, MAX_PROMPT_SIZE);
	while (ft_strlen(newpwd) > 1)
	{
		prev_len = ft_strlen(newpwd);
		ft_remove_last_dir(newpwd);
		if (ft_strlen(newpwd) == prev_len)
			break ;
		if (stat(newpwd, &st) == 0 && S_ISDIR(st.st_mode))
		{
			if (chdir(newpwd) == 0)
				return (1);
		}
	}
	ft_strlcpy(newpwd, "/", MAX_PROMPT_SIZE);
	if (chdir("/") == 0)
		return (1);
	return (0);
}

int	ft_builtin_cd(char **args, t_data *data)
{
	char		oldpwd[MAX_PROMPT_SIZE];
	char		newpwd[MAX_PROMPT_SIZE];
	char		*path;
	char		*oldpwd_str;
	char		*pwd_str;
	int			i;
	struct stat	st;
	char *pwd_env;
	
	i = 0;
	if (!getcwd(oldpwd, sizeof(oldpwd)))
	{
		pwd_env = NULL;
		while (data->env[i])
		{
			if (ft_strncmp(data->env[i], "PWD=", 4) == 0)
			{
				pwd_env = data->env[i] + 4;
				break ;
			}
			i++;
		}
		if (pwd_env)
			ft_strlcpy(oldpwd, pwd_env, sizeof(oldpwd));
		else
			oldpwd[0] = '\0';
		i = 0;
	}
	if (args[1] && args[2])
	{
		fprintf(stderr, "minishell: cd: too many arguments\n");
		return (1);
	}
	if (!args[1] || !args[1][0])
	{
		path = getenv("HOME");
		if (!path)
		{
			fprintf(stderr, "minishell: cd: HOME not set\n");
			return (1);
		}
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		path = NULL;
		while (data->env[i] && !path)
		{
			if (ft_strncmp(data->env[i], "OLDPWD=", 7) == 0)
				path = data->env[i] + 7;
			i++;
		}
		if (!path)
		{
			fprintf(stderr, "minishell: cd: OLDPWD not set\n");
			return (1);
		}
		printf("%s\n", path);
	}
	else
		path = args[1];
	if (stat(path, &st) == 0 && !S_ISDIR(st.st_mode))
	{
		fprintf(stderr, "minishell: cd: %s: Not a directory\n", path);
		return (1);
	}
	if (stat(path, &st) != 0 && ft_strcmp(path, "..") != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		return (1);
	}
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putendl_fd(": Permission denied", STDERR_FILENO);
		return (1);
	}
	if (!getcwd(newpwd, sizeof(newpwd)))
	{
		if (ft_strcmp(path, "..") == 0 && oldpwd[0])
		{
			if (!find_valid_parent(oldpwd, newpwd))
			{
				fprintf(stderr, "minishell: cd: error retrieving current directory: "
					"getcwd: cannot access parent directories: "
					"No such file or directory\n");
				return (1);
			}
			fprintf(stderr, "minishell: cd: warning: could not get current directory: "
				"moved to nearest valid parent\n");
		}
		else if (oldpwd[0])
			ft_strjoin_path(oldpwd, path, newpwd);
		else
		{
			ft_putendl_fd("minishell: cd: error retrieving current directory", STDERR_FILENO);
			return (1);
		}
	}
	if (oldpwd[0])
	{
		oldpwd_str = ft_strjoin("OLDPWD=", oldpwd);
		if (!oldpwd_str)
			return (1);
		data->env = add_or_update_env(data->env, oldpwd_str);
		free(oldpwd_str);
	}
	pwd_str = ft_strjoin("PWD=", newpwd);
	if (!pwd_str)
		return (1);
	data->env = add_or_update_env(data->env, pwd_str);
	free(pwd_str);
	return (0);
}
