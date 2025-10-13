/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:42:58 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/13 13:01:45 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"\

static void	fatal_error(char *msg)
{
	ft_putendl_fd(msg, 1);
	exit(EXIT_FAILURE);
}

static void	cpy_env(t_data *data, char **env)
{
	int i;

	i = 0;
	while (env[i])
		i++;
	data->env = malloc(sizeof(char *) * (i + 1));
	if (!data->env)
		ft_fatal_error("Error allocating memory for env");
	i = 0;
	while (env[i])
	{
		data->env[i] = ft_strdup(env[i]);
		if (!data->env[i])
			ft_fatal_error("Error duplicating env string");
		i++;
	}
	data->env[i] = NULL;
}

void	init_data(t_data *data, char **env)
{
	data->tokens = NULL;
	data->tokenizer->start = NULL;
	data->tokenizer->end = NULL;
	data->env = NULL;
	cpy_env(data, env);
}
