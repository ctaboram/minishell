/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctaboada <ctaboada@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:01:25 by ctaboada          #+#    #+#             */
/*   Updated: 2025/09/18 12:57:27 by ctaboada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_is_sorted(char **env, int count)
{
	int	i;

	i = 0;
	while (i < count - 1)
	{
		if (ft_strcmp(env[i], env[i + 1]) > 0)
			return (0);
		i++;
	}
	return (1);
}


static void	ft_sort_env(char **env, int count)
{
	int		i;
	char	*tmp;

	while (!ft_is_sorted(env, count))
	{
		i = 0;
		while (i < count - 1)
		{
			if (ft_strcmp(env[i], env[i + 1]) > 0)
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
	int	i;

	i = 0;
	if(!env)
		return;
	while(env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

static int	ft_export_print(char **env)
{
	int		i, count, key_len;
	char	**sorted;
	char	*equal;

	count = 0;
	while (env[count])
		count++;
	sorted = dup_env(env); // ya la tienes implementada
	if (!sorted)
		return (1);
	ft_sort_env(sorted, count);
	i = 0;
	while (sorted[i])
	{
		equal = ft_strchr(sorted[i], '=');
		if (equal)
		{
			key_len = equal - sorted[i];
			printf("declare -x %.*s=\"%s\"\n", key_len, sorted[i], equal + 1);
		}
		else
			printf("declare -x %s\n", sorted[i]);
		i++;
	}
	ft_free_env(sorted); // para liberar tu copia
	return (0);
}


int ft_builtin_export(char **arg,char **env)
{
	int		i;
	int		return_status;
	
	i = 1;
	return_status = 0;
	if(!arg[1])
		return (ft_export_print(env));
	while(arg[i])
	{
		if(!is_valid_identifier(arg[i])) // funcion para comprobar que el argumento sea valido
		{
			printf("export: `%s': not a valid identifier\n", arg[i]);
			return_status = 1;
		}
		else
		{
			env = add_or_update_env(env,arg[i]); //funcion para añadir o para actualizar el env
			if(!env)
				return(1);
		}
		i++;
	}
	return (return_status);
}
