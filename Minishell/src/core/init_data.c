/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 10:06:47 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/11/13 10:16:04 by nacuna-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	init_expand_struct(t_data *data)
{
	data->expand.result = NULL;
	data->expand.var = NULL;
	data->expand.value = NULL;
	data->expand.i = 0;
	data->expand.start = 0;
	data->expand.input = NULL;
	data->expand.input_expanded = NULL;
	data->expand.env = data->env;
	data->expand.exit_status = 0;
}

static void	init_tokenizer_struct(t_data *data)
{
	data->tokenizer.start = NULL;
	data->tokenizer.end = NULL;
	data->tokenizer.input_to_tokenize = NULL;
	data->tokenizer.tokens = NULL;
}

static void	init_parser_struct(t_data *data)
{
	data->parser.current = NULL;
	data->parser.head = NULL;
	data->parser.cmd = NULL;
	data->parser.tokens = NULL;
	data->parser.cmds_list = NULL;
}

static void	init_executor_struct(t_data *data)
{
	data->execute.cmds_list = NULL;
	data->execute.env = data->env;
	data->execute.exit_status = 0;
	data->execute.last_pid = 0;
	data->execute.pipe_fds[0] = 0;
	data->execute.pipe_fds[1] = 0;
	data->execute.in_fd = 0;
}

void	init_data(t_data *data, char **env)
{
	data->input = NULL;
	data->exit_status = 0;
	data->env = NULL;
	cpy_env(data, env);
	increment_shlvl(data);
	init_expand_struct(data);
	init_tokenizer_struct(data);
	init_parser_struct(data);
	init_executor_struct(data);
}
