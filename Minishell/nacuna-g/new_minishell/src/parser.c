/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 12:55:42 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/07 09:13:23 by nacuna-g         ###   ########.fr       */
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

int	parser_tokens(t_data *data)
{
	t_parser	parser;
	int			status;

	status = 0;
	init_parser(&parser,data->tokens);
	while (parser.current && parser.current->type != TOKEN_EOF)
	{
		if (parser.current->type == TOKEN_WORD
			|| parser.current->type == TOKEN_QUOTE_SINGLE
			|| parser.current->type == TOKEN_QUOTE_DOUBLE)
			status = handler_word_parser(&parser);
		else if (parser.current->type == TOKEN_PIPE)
			status = handler_pipe_parser(&parser);
		else if (parser.current->type == TOKEN_REDIR_OUT
			|| parser.current->type == TOKEN_REDIR_APPEND)
			status = handle_redir_out_parser(&parser);
		else if (parser.current->type == TOKEN_REDIR_IN
			|| parser.current->type == TOKEN_HEREDOC)
			status = handle_redir_in_parser(&parser);
		if (status)
			return (status);
		parser.current = parser.current->next;
	}
	data->cmds_list = parser.head;
	return (PARSER_OK);
}
