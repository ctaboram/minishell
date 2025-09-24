/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:42:58 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/24 13:20:39 by nacuna-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_data	*init_data(void)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
	{
		printf("Error allocating memory for env\n");
		return (NULL);
	}
	data->env = NULL;
	return (data);
}

void	cpy_env(t_data *data, char **env)
{
	int i;

	i = 0;
	while (env[i])
		i++;
	data->env = malloc(sizeof(char *) * (i + 1));
	if (!data->env)
	{
		printf("Error allocating memory for env\n");
		return ;
	}
	i = 0;
	while (env[i])
	{
		data->env[i] = ft_strdup(env[i]);
		if (!data->env[i])
		{
			printf("Error copying env\n");
			return ;
		}
		i++;
	}
	data->env[i] = NULL;
	return ;
}
