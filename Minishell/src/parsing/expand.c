/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 11:50:45 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/22 10:45:14 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_env_value(const char *var, char **env)
{
	char	*value;
	int		i;
	int		var_len;

	if (!var || !env)
		return (ft_strdup(""));
	var_len = ft_strlen(var);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, var_len) == 0 && env[i][var_len] == '=')
			return (ft_strdup(env[i] + var_len + 1));
		i++;
	}
	value = getenv(var);
	if (!value)
		return (ft_strdup(""));
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

static t_expand_error	get_var_name(t_expand *ex, char *word)
{
	int	start;

	start = ex->i;
	if (word[ex->i] == '?')
	{
		ex->i++;
		ex->var = ft_strdup("?");
		if (!ex->var)
			return (EXPAND_MEMORY_ALLOC);
		return (EXPAND_OK);
	}
	while (word[ex->i] && (ft_isalnum(word[ex->i]) || word[ex->i] == '_'))
		ex->i++;
	ex->var = ft_strndup(word + start, ex->i - start);
	if (!ex->var)
		return (EXPAND_MEMORY_ALLOC);
	return (EXPAND_OK);
}

t_expand_error	expand_variable(t_data *data)
{
	char	*tmp;
	int		status;

	status = get_var_name(&data->expand, data->expand.input);
	if (status != EXPAND_OK)
		return (status);
	if (ft_strcmp(data->expand.var, "?") == 0)
		data->expand.value = ft_itoa(data->expand.exit_status);
	else
		data->expand.value = get_env_value(data->expand.var, data->expand.env);
	if (!data->expand.value)
		return (EXPAND_MEMORY_ALLOC);
	tmp = ft_strjoin_free(data->expand.result, data->expand.value);
	if (!tmp)
		return (EXPAND_MEMORY_ALLOC);
	data->expand.result = tmp;
	free(data->expand.var);
	free(data->expand.value);
	return (EXPAND_OK);
}

static t_expand_error	process_text_and_vars(t_data *data)
{
	t_expand_error	status;

	while (data->expand.input[data->expand.i])
	{
		data->expand.start = data->expand.i;
		while (data->expand.input[data->expand.i]
			&& data->expand.input[data->expand.i] != '$')
			data->expand.i++;
		if (data->expand.i > data->expand.start)
		{
			status = append_segment(&data->expand,
					data->expand.input + data->expand.start,
					data->expand.i - data->expand.start);
			if (status != EXPAND_OK)
				return (status);
		}
		if (data->expand.input[data->expand.i] == '$')
		{
			status = handle_dollar(data);
			if (status != EXPAND_OK)
				return (status);
		}
	}
	return (EXPAND_OK);
}

t_expand_error	expand_word(t_data *data)
{
	t_expand_error	status;

	init_expand(data);
	if (!data->expand.result)
		return (EXPAND_MEMORY_ALLOC);
	status = process_text_and_vars(data);
	if (status != EXPAND_OK)
		return (status);
	data->expand.input_expanded = data->expand.result;
	return (EXPAND_OK);
}
