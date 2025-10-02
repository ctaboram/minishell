/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 12:55:42 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/02 12:12:54 by nacuna-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd	*parser_tokens(t_token *tokens)
{
	t_parser	parser;

	init_parser(&parser,tokens);
	while (parser.current && parser.current->type != TOKEN_EOF)
	{
		if (parser.current->type == TOKEN_WORD
			|| parser.current->type == TOKEN_QUOTE_SINGLE
			|| parser.current->type == TOKEN_QUOTE_DOUBLE)
		{
			append_arg(parser.cmd, parser.current->value);
		}
		else if (parser.current->type == TOKEN_PIPE)
		{
			parser.cmd->next = new_cmd();
			parser.cmd = parser.cmd->next;
		}
		else if (parser.current->type == TOKEN_REDIR_OUT
			|| parser.current->type == TOKEN_REDIR_APPEND)
		{
			parser.current = parser.current->next;
			parser.cmd->redir_out = ft_strdup(parser.current->value);
			parser.cmd->append = (parser.current->type == TOKEN_REDIR_APPEND);
			// if (parser.current->type == TOKEN_REDIR_APPEND)
			// 	parser.cmd->append = 1;
			// else
			// 	parser.cmd->append = 0;
		}
		else if (parser.current->type == TOKEN_REDIR_IN
			|| parser.current->type == TOKEN_HEREDOC)
		{
			parser.current = parser.current->next;
			parser.cmd->redir_in = ft_strdup(parser.current->value);
		}
		parser.current = parser.current->next;
	}
	return (parser.head);
}
