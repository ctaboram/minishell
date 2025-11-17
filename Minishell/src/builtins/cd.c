/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:42:44 by ctaboada          #+#    #+#             */
/*   Updated: 2025/10/16 12:11:18 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <sys/stat.h>

static char	*get_target_path(char **args, t_data *data)
{
	char	*path;

	if (!args[1] || !args[1][0])
		path = getenv("HOME");
	else if (ft_strcmp(args[1], "-") == 0)
		path = get_oldpwd_path(data);
	else
		path = args[1];
	if (args[1] && ft_strcmp(args[1], "-") == 0 && path)
		printf("%s\n", path);
	return (path);
}

static int	validate_path(char *path, char **args)
{
	struct stat	st;

	if (!path && (!args[1] || !args[1][0]))
	{
		fprintf(stderr, "minishell: cd: HOME not set\n");
		return (1);
	}
	if (!path && args[1] && ft_strcmp(args[1], "-") == 0)
	{
		fprintf(stderr, "minishell: cd: OLDPWD not set\n");
		return (1);
	}
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
	return (0);
}

static int	update_env_pwd(t_data *data, char *oldpwd, char *newpwd)
{
	char	*oldpwd_str;
	char	*pwd_str;

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

static int	handle_getcwd_fail(char *oldpwd, char *newpwd, char *path)
{
	if (ft_strcmp(path, "..") == 0 && oldpwd[0])
	{
		if (!find_valid_parent(oldpwd, newpwd))
		{
			fprintf(stderr, "minishell: cd: error retrieving "
				"current directory: getcwd: cannot access "
				"parent directories: No such file or directory\n");
			return (1);
		}
		fprintf(stderr, "minishell: cd: warning: could not "
			"get current directory: moved to nearest valid parent\n");
	}
	else if (oldpwd[0])
		ft_strjoin_path(oldpwd, path, newpwd);
	else
	{
		ft_putendl_fd("minishell: cd: error retrieving current directory",
			STDERR_FILENO);
		return (1);
	}
	return (0);
}

int	ft_builtin_cd(char **args, t_data *data)
{
	char	oldpwd[MAX_PROMPT_SIZE];
	char	newpwd[MAX_PROMPT_SIZE];
	char	*path;

	if (!getcwd(oldpwd, sizeof(oldpwd)))
		get_oldpwd(oldpwd, data);
	if (args[1] && args[2])
	{
		fprintf(stderr, "minishell: cd: too many arguments\n");
		return (1);
	}
	path = get_target_path(args, data);
	if (validate_path(path, args))
		return (1);
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putendl_fd(": Permission denied", STDERR_FILENO);
		return (1);
	}
	if (!getcwd(newpwd, sizeof(newpwd)))
		if (handle_getcwd_fail(oldpwd, newpwd, path))
			return (1);
	return (update_env_pwd(data, oldpwd, newpwd));
}
