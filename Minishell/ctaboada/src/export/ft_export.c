	/* ************************************************************************** */
	/*                                                                            */
	/*                                                        :::      ::::::::   */
	/*   ft_export.c                                        :+:      :+:    :+:   */
	/*                                                    +:+ +:+         +:+     */
	/*   By: ctaboada <ctaboada@student.42malaga.com    +#+  +:+       +#+        */
	/*                                                +#+#+#+#+#+   +#+           */
	/*   Created: 2025/09/16 12:01:25 by ctaboada          #+#    #+#             */
	/*   Updated: 2025/10/02 12:41:10 by ctaboada         ###   ########.fr       */
	/*                                                                            */
	/* ************************************************************************** */

	#include "../../includes/minishell.h"

	static int ft_is_sorted(char **env, int count)
	{
	int i;
	if (!env)
		return (1);

	i = 0;
	while (i < count - 1)
	{
		if (env[i] && env[i + 1] && ft_strcmp(env[i], env[i + 1]) > 0)
			return (0);
		i++;
	}
	return (1);
	}

	static void ft_sort_env(char **env, int count)
	{
	int i;
	char *tmp;

	if (!env)
		return;

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

	void ft_free_env(char **env)
	{
	int i;

	if (!env)
		return;

	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
	}

	void ft_export_print(char **env)
	{
	int i;
	char **sorted;
	char *equal;
	char *key;

	if (!env)
		return;

	sorted = dup_env(env);
	if (!sorted)
		return;

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

	char **ft_builtin_export(char **arg, char **env)
	{
	int i = 1;
	char **new_env = NULL;
	char **old_env = env;
	char **env_copy = NULL;

	if (!arg[1])
	{
		env_copy = dup_env(env);
		if (!env_copy)
		{
			printf("export: failed to duplicate environment\n");
			return (env);
		}
		ft_export_print(env_copy);
		ft_free_env(env_copy);
		return (env);
	}

	while (arg[i])
	{
		if (!is_valid_identifier(arg[i]))
			printf("export: `%s': not a valid identifier\n", arg[i]);
		else
		{
			new_env = add_or_update_env(env, arg[i]);
			if (!new_env)
			{
				printf("export: failed to add/update variable\n");
				return (env);
			}
			if (new_env != env)
			{
				old_env = env;
				env = new_env;
				ft_free_env(old_env);
			}
		}
		i++;
	}

	return (new_env);
	}