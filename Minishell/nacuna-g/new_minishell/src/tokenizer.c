/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 11:33:17 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/09 11:23:15 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_redirect_out(t_tokenizer *tokenizer, t_data *data)
{
	if (!data->tokens || data->tokens->type == TOKEN_PIPE)
		return (TOK_SYNTAX_REDIR);
	if (*(tokenizer->end + 1) == '>')
	{
		add_token(&data->tokens, create_token(">>", TOKEN_REDIR_APPEND));
		tokenizer->end += 2;
	}
	else
	{
		add_token(&data->tokens, create_token(">", TOKEN_REDIR_IN));
		tokenizer->end++;
	}
	if (*tokenizer->end == '\0')
		return (TOK_SYNTAX_REDIR);
	tokenizer->start = tokenizer->end;
	return (TOK_OK);
}

static int	handle_redirect_in(t_tokenizer *tokenizer, t_data *data)
{
	if (!data->tokens || data->tokens->type == TOKEN_PIPE)
		return (TOK_SYNTAX_REDIR);
	if (*(tokenizer->end + 1) == '<')
	{
		add_token(&data->tokens, create_token("<<", TOKEN_HEREDOC));
		tokenizer->end += 2;
	}
	else
	{
		add_token(&data->tokens, create_token("<", TOKEN_REDIR_OUT));
		tokenizer->end++;
	}
	if (*tokenizer->end == '\0')
		return (TOK_SYNTAX_REDIR);
	tokenizer->start = tokenizer->end;
	return (TOK_OK);
}

static int	handle_pipe(t_tokenizer *tokenizer, t_data *data)
{
	if (!data->tokens || data->tokens->type == TOKEN_PIPE)
		return (TOK_SYNTAX_PIPE);
	tokenizer->end++;
	if (*tokenizer->end == '\0')
		return (TOK_SYNTAX_PIPE);
	add_token(&data->tokens, create_token("|", TOKEN_PIPE));
	tokenizer->start = tokenizer->end;
	return (TOK_OK);
}

static int	handle_word(t_tokenizer *tokenizer, t_data *data)
{
	char *value;

	while (*tokenizer->end && !ft_isspace(*tokenizer->end) &&
			*tokenizer->end != '\'' && *tokenizer->end != '"')
		tokenizer->end++;
	if (tokenizer->end > tokenizer->start)
	{
		value = ft_strndup(tokenizer->start, tokenizer->end - tokenizer->start);
		if (!value)
			return (TOK_MEMORY_ALLOC);
		add_token(&data->tokens, create_token(value, TOKEN_WORD));
		free(value);
	}
	tokenizer->start = tokenizer->end;
	return (TOK_OK);
}

static int	handle_quote(t_tokenizer *tokenizer, t_data *data, char quote)
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
		free_tokens(data->tokens);
		data->tokens = NULL;
		tokenizer->start = tokenizer->end;
		return (TOK_MEMORY_ALLOC);
	}
	if (quote == '\'')
		add_token(&data->tokens, create_token(value, TOKEN_QUOTE_SINGLE));
	else
		add_token(&data->tokens, create_token(value, TOKEN_QUOTE_DOUBLE));
	free(value);
	tokenizer->end++;
	tokenizer->start = tokenizer->end;
	return (TOK_OK);
}

t_tokenizer_error	tokenizer(t_data *data, t_tokenizer	*tokenizer)
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
		else if(*tokenizer->end == '|')
			status = handle_pipe(tokenizer, data);
		else if(*tokenizer->end == '>')
			status = handle_redirect_out(tokenizer, data);
		else if(*tokenizer->end == '<')
			status = handle_redirect_in(tokenizer, data);
		else
			status = handle_word(tokenizer, data);
		if(status)
			return (status);
	}
	add_token(&data->tokens, create_token(NULL, TOKEN_EOF));
	return (TOK_OK);
}
