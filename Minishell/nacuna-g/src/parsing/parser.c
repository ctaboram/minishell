/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 12:55:42 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/14 12:44:32 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handler_word_parser(t_parser *parser)
{
	if (append_arg(parser->cmd, parser->current->value))
				return (PARSER_MEMORY_ALLOC);
	return (PARSER_OK);
}

static int	handler_pipe_parser(t_parser *parser)
{
	if (!parser->cmd->av
		|| (parser->current->next && parser->current->next->type == TOKEN_PIPE)
		|| parser->current->next == NULL)
		return (PARSER_SYNTAX_PIPE);
	parser->cmd->next = new_cmd();
	if (!parser->cmd->next)
		return (PARSER_MEMORY_ALLOC);
	parser->cmd = parser->cmd->next;
	return (PARSER_OK);
}

static int	handle_redir_out_parser(t_parser *parser)
{
	if (!parser->current->next || parser->current->next->type != TOKEN_WORD)
		return (PARSER_SYNTAX_REDIR);
	if (parser->current->type == TOKEN_REDIR_APPEND)
		parser->cmd->is_append = 1;
	else
		parser->cmd->is_append = 0;
	parser->current = parser->current->next;
	parser->cmd->redir_out = ft_strdup(parser->current->value);
	if (!parser->cmd->redir_out)
		return (PARSER_MEMORY_ALLOC);
	return (PARSER_OK);
}

static int	handle_redir_in_parser(t_parser *parser)
{
	if (!parser->current->next || parser->current->next->type != TOKEN_WORD)
		return (PARSER_SYNTAX_REDIR);
	parser->current = parser->current->next;
	parser->cmd->redir_in = ft_strdup(parser->current->value);
	if (!parser->cmd->redir_in)
		return (PARSER_MEMORY_ALLOC);
	return (PARSER_OK);
}

t_parser_error	parser_tokens(t_data *data)
{
	t_parser_error	status;

	status = 0;
	// Inicializar la estructura parser con los tokens del tokenizer
	data->parser.tokens = data->tokenizer.tokens;
	init_parser(&data->parser, data->parser.tokens);
	while (data->parser.current && data->parser.current->type != TOKEN_EOF)
	{
		if (data->parser.current->type == TOKEN_WORD
			|| data->parser.current->type == TOKEN_QUOTE_SINGLE
			|| data->parser.current->type == TOKEN_QUOTE_DOUBLE)
			status = handler_word_parser(&data->parser);
		else if (data->parser.current->type == TOKEN_PIPE)
			status = handler_pipe_parser(&data->parser);
		else if (data->parser.current->type == TOKEN_REDIR_OUT
			|| data->parser.current->type == TOKEN_REDIR_APPEND)
			status = handle_redir_out_parser(&data->parser);
		else if (data->parser.current->type == TOKEN_REDIR_IN
			|| data->parser.current->type == TOKEN_HEREDOC)
			status = handle_redir_in_parser(&data->parser);
		if (status)
			return (status);
		data->parser.current = data->parser.current->next;
	}
	data->parser.cmds_list = data->parser.head;
	return (PARSER_OK);
}
