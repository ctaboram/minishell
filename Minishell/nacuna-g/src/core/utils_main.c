/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:42:58 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/22 10:30:05 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
		fatal_error("Error allocating memory for env");
	i = 0;
	while (env[i])
	{
		data->env[i] = ft_strdup(env[i]);
		if (!data->env[i])
			fatal_error("Error duplicating env string");
		i++;
	}
	data->env[i] = NULL;
}

void	init_data(t_data *data, char **env)
{
	// Inicializar variables principales
	data->input = NULL;
	data->exit_status = 0;
	data->env = NULL;
	cpy_env(data, env);
	
	// Inicializar estructura expand
	data->expand.result = NULL;
	data->expand.var = NULL;
	data->expand.value = NULL;
	data->expand.i = 0;
	data->expand.start = 0;
	data->expand.input = NULL;
	data->expand.input_expanded = NULL;
	data->expand.env = data->env;
	data->expand.exit_status = 0;
	
	// Inicializar estructura tokenizer
	data->tokenizer.start = NULL;
	data->tokenizer.end = NULL;
	data->tokenizer.input_to_tokenize = NULL;
	data->tokenizer.tokens = NULL;
	
	// Inicializar estructura parser
	data->parser.current = NULL;
	data->parser.head = NULL;
	data->parser.cmd = NULL;
	data->parser.tokens = NULL;
	data->parser.cmds_list = NULL;
	
	// Inicializar estructura execute
	data->execute.cmds_list = NULL;
	data->execute.env = data->env;
	data->execute.exit_status = 0;
	data->execute.last_pid = 0;
	data->execute.pipe_fds[0] = 0;
	data->execute.pipe_fds[1] = 0;
	data->execute.in_fd = 0;
}
