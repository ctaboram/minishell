/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:01:25 by ctaboada          #+#    #+#             */
/*   Updated: 2025/10/16 12:13:14 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_is_sorted(char **env, int count)
{
	int	i;

	i = 0;
	if (!env)
		return (1);
	while (i < count - 1)
	{
		if (env[i] && env[i + 1] && ft_strcmp(env[i], env[i + 1]) > 0)
			return (0);
		i++;
	}
	return (1);
}

static void	ft_sort_env(char **env, int count)
{
	int		i;
	char	*tmp;

	if (!env)
		return ;
	while (!ft_is_sorted(env, count))
	{
		i = 0;
		while (i < count - 1)
		{
			if (env[i] && env[i + 1] && ft_strcmp(env[i], env[i + 1]) > 0)
			{
				tmp = env[i];
				env[i] = env[i + 1];
				env[i + 1] = tmp;
			}
			i++;
		}
	}
}

void	ft_free_env(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void	ft_export_print(char **env)
{
	int		i;
	char	**sorted;
	char	*equal;
	char	*key;

	if (!env)
		return ;
	sorted = dup_env(env);
	if (!sorted)
	{
		ft_putendl_fd("minishell: export: memory allocation failed",
			STDERR_FILENO);
		return ;
	}
	i = 0;
	while (sorted[i])
		i++;
	ft_sort_env(sorted, i);
	i = 0;
	while (sorted[i])
	{
		equal = ft_strchr(sorted[i], '=');
		if (equal)
		{
			key = ft_substr(sorted[i], 0, equal - sorted[i]);
			if (key)
			{
				printf("declare -x %s=\"%s\"\n", key, equal + 1);
				free(key);
			}
		}
		else
			printf("declare -x %s\n", sorted[i]);
		i++;
	}
	ft_free_env(sorted);
}

char	**ft_builtin_export(char **arg, char **env)
{
	int		i;
	char	**new_env;
	char	**env_copy;
	char	**temp_env;

	i = 1;
	new_env = env;
	env_copy = NULL;
	temp_env = NULL;
	if (!arg || !env)
		return (env);
	if (!arg[1])
	{
		env_copy = dup_env(env);
		if (!env_copy)
		{
			ft_putendl_fd("minishell: export: failed to duplicate environment",
				STDERR_FILENO);
			return (env);
		}
		ft_export_print(env_copy);
		ft_free_env(env_copy);
		return (env);
	}
	while (arg[i])
	{
		if (!is_valid_identifier(arg[i]))
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(arg[i], STDERR_FILENO);
			ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		}
		else
		{
			temp_env = add_or_update_env(new_env, arg[i]);
			if (!temp_env)
				ft_putendl_fd("minishell: export: error to add/update variable",
					STDERR_FILENO);
			else
				new_env = temp_env;
		}
		i++;
	}
	return (new_env);
}
