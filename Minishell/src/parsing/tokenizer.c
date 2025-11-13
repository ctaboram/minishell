/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 11:33:17 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/15 11:46:33 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_redirect_out(t_tokenizer *tokenizer)
{
	if (!tokenizer->tokens || tokenizer->tokens->type == TOKEN_PIPE)
		return (TOK_SYNTAX_REDIR);
	if (*(tokenizer->end + 1) == '>')
	{
		add_token(&tokenizer->tokens, create_token(">>", TOKEN_REDIR_APPEND));
		tokenizer->end += 2;
	}
	else
	{
		add_token(&tokenizer->tokens, create_token(">", TOKEN_REDIR_OUT));
		tokenizer->end++;
	}
	if (*tokenizer->end == '\0')
		return (TOK_SYNTAX_REDIR);
	tokenizer->start = tokenizer->end;
	return (TOK_OK);
}

static int	handle_redirect_in(t_tokenizer *tokenizer)
{
	if (!tokenizer->tokens || tokenizer->tokens->type == TOKEN_PIPE)
		return (TOK_SYNTAX_REDIR);
	if (*(tokenizer->end + 1) == '<')
	{
		add_token(&tokenizer->tokens, create_token("<<", TOKEN_HEREDOC));
		tokenizer->end += 2;
	}
	else
	{
		add_token(&tokenizer->tokens, create_token("<", TOKEN_REDIR_IN));
		tokenizer->end++;
	}
	if (*tokenizer->end == '\0')
		return (TOK_SYNTAX_REDIR);
	tokenizer->start = tokenizer->end;
	return (TOK_OK);
}

static int	handle_pipe(t_tokenizer *tokenizer)
{
	if (!tokenizer->tokens || tokenizer->tokens->type == TOKEN_PIPE)
		return (TOK_SYNTAX_PIPE);
	tokenizer->end++;
	if (*tokenizer->end == '\0')
		return (TOK_SYNTAX_PIPE);
	add_token(&tokenizer->tokens, create_token("|", TOKEN_PIPE));
	tokenizer->start = tokenizer->end;
	return (TOK_OK);
}

static int	handle_word(t_tokenizer *tokenizer)
{
	char *value;

	while (*tokenizer->end && !ft_isspace(*tokenizer->end) &&
			*tokenizer->end != '\'' && *tokenizer->end != '"' &&
			*tokenizer->end != '|' && *tokenizer->end != '<' && *tokenizer->end != '>')
		tokenizer->end++;
	if (tokenizer->end > tokenizer->start)
	{
		value = ft_strndup(tokenizer->start, tokenizer->end - tokenizer->start);
		if (!value)
			return (TOK_MEMORY_ALLOC);
		add_token(&tokenizer->tokens, create_token(value, TOKEN_WORD));
		free(value);
	}
	tokenizer->start = tokenizer->end;
	return (TOK_OK);
}

static int	handle_quote(t_tokenizer *tokenizer, char quote)
{
	char *value;

	tokenizer->end++;
	while (*tokenizer->end && *tokenizer->end != quote)
		tokenizer->end++;
	if (*tokenizer->end != quote)
	{
		tokenizer->start = tokenizer->end;
		return (TOK_UNCLOSED_QUOTE);
	}
	value = ft_strndup(tokenizer->start + 1, tokenizer->end - tokenizer->start - 1);
	if (!value)
	{
		tokenizer->start = tokenizer->end;
		return (TOK_MEMORY_ALLOC);
	}
	if (quote == '\'')
		add_token(&tokenizer->tokens, create_token(value, TOKEN_QUOTE_SINGLE));
	else
		add_token(&tokenizer->tokens, create_token(value, TOKEN_QUOTE_DOUBLE));
	free(value);
	tokenizer->end++;
	tokenizer->start = tokenizer->end;
	return (TOK_OK);
}

t_tokenizer_error	tokenizer(t_data *data)
{
	t_tokenizer_error	status;

	if (!data)
		return (TOK_MEMORY_ALLOC);
	// Usar input_expanded si está disponible, sino usar input original
	if (data->expand.input_expanded)
		data->tokenizer.input_to_tokenize = data->expand.input_expanded;
	else
		data->tokenizer.input_to_tokenize = data->input;
	if (!data->tokenizer.input_to_tokenize)
		return (TOK_MEMORY_ALLOC);
	status = 0;
	data->tokenizer.start = data->tokenizer.input_to_tokenize;
	data->tokenizer.end = data->tokenizer.input_to_tokenize;
	data->tokenizer.tokens = NULL;
	while (*data->tokenizer.start)
	{
		while (*data->tokenizer.start && ft_isspace(*data->tokenizer.start))
			data->tokenizer.start++;
		if (!*data->tokenizer.start)
			break ;
		data->tokenizer.end = data->tokenizer.start;
		if (*data->tokenizer.end == '\'' || *data->tokenizer.end == '"')
			status = handle_quote(&data->tokenizer, *data->tokenizer.end);
		else if(*data->tokenizer.end == '|')
			status = handle_pipe(&data->tokenizer);
		else if(*data->tokenizer.end == '>')
			status = handle_redirect_out(&data->tokenizer);
		else if(*data->tokenizer.end == '<')
			status = handle_redirect_in(&data->tokenizer);
		else
			status = handle_word(&data->tokenizer);
		if(status)
			return (status);
	}
	add_token(&data->tokenizer.tokens, create_token(NULL, TOKEN_EOF));
	return (TOK_OK);
}
