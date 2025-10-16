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

int	ft_builtin_cd(char **args, t_data *data)
{
	char	oldpwd[MAX_PROMPT_SIZE];
	char	newpwd[MAX_PROMPT_SIZE];
	char	*path;
	char	*oldpwd_str;
	char	*pwd_str;
	int		i;

	i = 0;
	if (!getcwd(oldpwd, sizeof(oldpwd)))
	{
		perror("cd: getcwd");
		return (1);
	}
	if (!args[1] || !args[1][0])
	{
		path = getenv("HOME");
		if (!path)
		{
			fprintf(stderr, "cd: HOME not set\n");
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
			fprintf(stderr, "cd: OLDPWD not set\n");
			return (1);
		}
		printf("%s\n", path);
	}
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	if (!getcwd(newpwd, sizeof(newpwd)))
	{
		if (ft_strcmp(path, "..") == 0)
			ft_remove_last_dir(oldpwd);
		else
			ft_strjoin_path(oldpwd, path, newpwd);
	}
	oldpwd_str = ft_strjoin("OLDPWD=", oldpwd);
	if (!oldpwd_str)
		return (1);
	data->env = add_or_update_env(data->env, oldpwd_str);
	free(oldpwd_str);
	pwd_str = ft_strjoin("PWD=", newpwd);
	if (!pwd_str)
		return (1);
	data->env = add_or_update_env(data->env, pwd_str);
	free(pwd_str);
	return (0);
}
