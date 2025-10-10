/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 11:50:45 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/10 12:16:24 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_env_value(const char *var)
{
	char	*value;

	value = getenv(var);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
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

static t_expand_error	append_segment(t_expand *ex, char *start, int len)
{
	char	*segment;
	char	*tmp;

	segment = ft_strndup(start, len);
	if (!segment)
		return (EXPAND_MEMORY_ALLOC);
	tmp = ft_strjoin_free(ex->result, segment);
	free(segment);
	if (!tmp)
		return (EXPAND_MEMORY_ALLOC);
	ex->result = tmp;
	return (EXPAND_OK);
}

t_expand_error	expand_word(t_data *data)
{
	t_expand	ex;
	char		*tmp;

	init_expand(&ex);
	if (!ex.result)
		return (EXPAND_MEMORY_ALLOC);
	while (data->input[ex.i])
	{
		ex.start = ex.i;
		while (data->input[ex.i] && data->input[ex.i] != '$')
			ex.i++;
		if (ex.i > ex.start)
		{
			data->exit_status = append_segment(&ex, data->input + ex.start, ex.i - ex.start);
			if (data->exit_status != EXPAND_OK)
				return (free(ex.result), data->exit_status);
		}
		if (data->input[ex.i] == '$')
		{
			ex.i++;
			if (data->input[ex.i] == '\0' || data->input[ex.i] == ' ')
			{
				ex.result = ft_strjoin_free(ex.result, "$");
				if (!ex.result)
					return (free(ex.result), EXPAND_MEMORY_ALLOC);
			}
			else
			{
				data->exit_status = get_var_name(&ex, data->input);
				if (data->exit_status != EXPAND_OK)
					return (free(ex.result), data->exit_status);
				if (ft_strcmp(ex.var, "?") == 0)
					ex.value = ft_itoa(data->exit_status);
				else
					ex.value = get_env_value(ex.var);
				if (!ex.value)
					return (free(ex.var), free(ex.result), EXPAND_MEMORY_ALLOC);
				tmp = ft_strjoin_free(ex.result, ex.value);
				if (!tmp)
					return (free(ex.var), free(ex.value), free(ex.result), EXPAND_MEMORY_ALLOC);
				ex.result = tmp;
				free(ex.var);
				free(ex.value);
			}
		}
	}
	data->input_expanded = ex.result;
	return (EXPAND_OK);
}
