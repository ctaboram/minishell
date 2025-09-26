/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 11:33:17 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/26 12:14:54 by nacuna-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*create_token(char *value, t_token_type type)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if(!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **tokens, t_token *token)
{
	t_token *current;
	
	if (!token)
		return ;
	if (!tokens)
	{
		*tokens = token;
		token->next = NULL;
		return ;
	}
	else
	{
		current = *tokens;
		while (current->next)
			current = current->next;
		current->next = token;
		token->next = NULL;
	}
}

static void	handle_quote(t_tokenizer tokenizer, t_data *data, char *quote)
{
	t_tokenizer	result;
	char		*value;

	result.start = tokenizer.start;
	result.end = tokenizer.end + 1;
	while (*result.end && *result.end != quote)
		result.end++;
	if (*result.end != quote)
	{
		ft_print_error(data, ERR_UNCLOSED_QUOTE, &data->exit_status);
		result.start = result.end;
		return (result);
	}
	value = ft_strndup(tokenizer.start, result.end - tokenizer.start + 1);
	if (!value)
	{
		free_tokens(data->tokens);
		data->tokens = NULL;
		ft_print_error(data, ERR_MEMORY_ALLOC, &data->exit_status);
		result.start = result.end;
		return (result);
	}
	if (quote == '\'')
		add_token(&data->tokens, create_token(value, TOKEN_QUOTE_SINGLE));
	else
		add_token(&data->tokens, create_token(value, TOKEN_QUOTE_DOUBLE));
	free (value);
	result.end++;
	result.start = result.end;
	return (result);
	
}

void	tokenizer(t_data *data)
{
	t_tokenizer	tokenizer;

	tokenizer.start = data->input;
	while (*tokenizer.start)
	{
		while (*tokenizer.start && ft_isspace(*tokenizer.start))
			tokenizer.start++;
		if (!*tokenizer.start)
			break ;
		tokenizer.end = tokenizer.start;
		if (*tokenizer.end == '\'' || *tokenizer.end == '"')
			handle_quote(tokenizer , data, *tokenizer.end);
		else
			tokenizer.start++;
	}
	return ;
}
