/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctaboada <ctaboada@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 11:22:13 by ctaboada          #+#    #+#             */
/*   Updated: 2025/10/14 11:25:12 by ctaboada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#include "../includes/minishell.h"

char	*find_command_path(char *cmd, char **envp)
{
	char	*path_env;
	char	**paths;
	char	full_path[PATH_MAX];
	int		i;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_env = NULL;
	for (i = 0; envp && envp[i]; i++)
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			path_env = envp[i] + 5;
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	i = 0;
	while (paths && paths[i])
	{
		snprintf(full_path, sizeof(full_path), "%s/%s", paths[i], cmd);
		if (access(full_path, X_OK) == 0)
		{
			char *res = ft_strdup(full_path);
			ft_free_split(paths);
			return (res);
		}
		i++;
	}
	ft_free_split(paths);
	return (NULL);
}

