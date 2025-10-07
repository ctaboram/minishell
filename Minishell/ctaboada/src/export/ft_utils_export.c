	/* ************************************************************************** */
	/*                                                                            */
	/*                                                        :::      ::::::::   */
	/*   ft_utils_export.c                                  :+:      :+:    :+:   */
	/*                                                    +:+ +:+         +:+     */
	/*   By: ctaboada <ctaboada@student.42malaga.com    +#+  +:+       +#+        */
	/*                                                +#+#+#+#+#+   +#+           */
	/*   Created: 2025/09/18 12:56:07 by ctaboada          #+#    #+#             */
	/*   Updated: 2025/10/02 12:41:49 by ctaboada         ###   ########.fr       */
	/*                                                                            */
	/* ************************************************************************** */

	#include "../../includes/minishell.h"

int is_valid_identifier(char *arg)
{
	int i;

	if (!arg || !arg[0] || (!ft_isalpha(arg[0]) && arg[0] != '_'))
		return (0);

	i = 1;
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

char **add_new_env(char **env, char *new_var)
{
	int count = 0;
	char **new_env;
	int i;

	while (env && env[count])
		count++;

	new_env = malloc((count + 2) * sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
		{
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[count] = ft_strdup(new_var);
	if (!new_env[count])
	{
		while (i--)
			free(new_env[i]);
		free(new_env);
		return (NULL);
	}
	new_env[count + 1] = NULL;

	return (new_env);
}

char **add_or_update_env(char **env, char *var)
{
	int i = 0;
	char *key;
	char *equal_sign;
	char **new_env;

	if (!var)
		return (env);

	equal_sign = ft_strchr(var, '=');
	if (!equal_sign)
		return (env);

	key = ft_substr(var, 0, equal_sign - var);
	if (!key)
		return (env);

	while (env && env[i])
	{
		if (ft_strncmp(env[i], key, ft_strlen(key)) == 0 &&
			(env[i][ft_strlen(key)] == '=' || env[i][ft_strlen(key)] == '\0'))
		{
			char *temp = ft_strdup(var);
			if (!temp)
			{
				free(key);
				return (env);
			}
			free(env[i]);
			env[i] = temp;
			free(key);
			return (env);
		}
		i++;
	}

	new_env = add_new_env(env, var);
	free(key);
	if (!new_env)
		return (env);
	return (new_env);
}
