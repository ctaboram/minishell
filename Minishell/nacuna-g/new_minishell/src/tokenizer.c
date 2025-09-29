/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 11:33:17 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/29 15:35:41 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int handle_word(t_tokenizer *tokenizer, t_data *data)
{
	char *value;

	while (*tokenizer->end && !ft_isspace(*tokenizer->end) &&
			*tokenizer->end != '\'' && *tokenizer->end != '"')
		tokenizer->end++;
	if (tokenizer->end > tokenizer->start)
	{
		value = ft_strndup(tokenizer->start, tokenizer->end - tokenizer->start);
		if (!value)
			return (ERR_MEMORY_ALLOC);
		add_token(&data->tokens, create_token(value, TOKEN_WORD));
		free(value);
	}
	tokenizer->start = tokenizer->end;
	return (ERR_OK);
}

static int handle_quote(t_tokenizer *tokenizer, t_data *data, char quote)
{
	char *value;

	tokenizer->end++;
	while (*tokenizer->end && *tokenizer->end != quote)
		tokenizer->end++;
	if (*tokenizer->end != quote)
	{
		tokenizer->start = tokenizer->end;
		return (ERR_UNCLOSED_QUOTE);
	}
	value = ft_strndup(tokenizer->start, tokenizer->end - tokenizer->start + 1);
	if (!value)
	{
		free_tokens(data->tokens);
		data->tokens = NULL;
		tokenizer->start = tokenizer->end;
		return (ERR_MEMORY_ALLOC);
	}
	if (quote == '\'')
		add_token(&data->tokens, create_token(value, TOKEN_QUOTE_SINGLE));
	else
		add_token(&data->tokens, create_token(value, TOKEN_QUOTE_DOUBLE));
	free(value);
	tokenizer->end++;
	tokenizer->start = tokenizer->end;
	return (ERR_OK);
}

int	tokenizer(t_data *data, t_tokenizer	*tokenizer)
{
	char		*value;
	int			status;

	tokenizer->start = data->input;
	status = 0;
	while (*tokenizer->start)
	{
		while (*tokenizer->start && ft_isspace(*tokenizer->start))
			tokenizer->start++;
		if (!*tokenizer->start)
			break ;
		tokenizer->end = tokenizer->start;
		if (*tokenizer->end == '\'' || *tokenizer->end == '"')
			status = handle_quote(tokenizer , data, *tokenizer->end);
		else
			status = handle_word(tokenizer, data);
		if(status)
			return (status);
	}
	return (ERR_OK);
}
