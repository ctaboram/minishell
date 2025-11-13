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
	int	i;

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

static void	increment_shlvl(t_data *data)
{
	int		i;
	int		shlvl;
	char	*shlvl_str;
	char	*new_var;

	i = 0;
	shlvl = 0;
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], "SHLVL=", 6) == 0)
		{
			shlvl = ft_atoi(data->env[i] + 6);
			if (shlvl < 0)
				shlvl = 0;
			shlvl++;
			shlvl_str = ft_itoa(shlvl);
			if (!shlvl_str)
				fatal_error("Error allocating memory for SHLVL");
			new_var = ft_strjoin("SHLVL=", shlvl_str);
			free(shlvl_str);
			if (!new_var)
				fatal_error("Error allocating memory for SHLVL");
			free(data->env[i]);
			data->env[i] = new_var;
			return ;
		}
		i++;
	}
	new_var = ft_strdup("SHLVL=1");
	if (!new_var)
		fatal_error("Error allocating memory for SHLVL");
	data->env = add_new_env(data->env, new_var);
	free(new_var);
}

void	init_data(t_data *data, char **env)
{
	data->input = NULL;
	data->exit_status = 0;
	data->env = NULL;
	cpy_env(data, env);
	increment_shlvl(data);
	data->expand.result = NULL;
	data->expand.var = NULL;
	data->expand.value = NULL;
	data->expand.i = 0;
	data->expand.start = 0;
	data->expand.input = NULL;
	data->expand.input_expanded = NULL;
	data->expand.env = data->env;
	data->expand.exit_status = 0;
	data->tokenizer.start = NULL;
	data->tokenizer.end = NULL;
	data->tokenizer.input_to_tokenize = NULL;
	data->tokenizer.tokens = NULL;
	data->parser.current = NULL;
	data->parser.head = NULL;
	data->parser.cmd = NULL;
	data->parser.tokens = NULL;
	data->parser.cmds_list = NULL;
	data->execute.cmds_list = NULL;
	data->execute.env = data->env;
	data->execute.exit_status = 0;
	data->execute.last_pid = 0;
	data->execute.pipe_fds[0] = 0;
	data->execute.pipe_fds[1] = 0;
	data->execute.in_fd = 0;
}
