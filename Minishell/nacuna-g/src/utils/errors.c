/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 11:49:06 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/13 13:01:34 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_tokenizer_error(t_tokenizer_error err)
{
	if (err == TOK_MEMORY_ALLOC)
		ft_putendl_fd("minishell: memory allocation error", 2);
	else if (err == TOK_UNCLOSED_QUOTE)
		ft_putendl_fd("minishell: syntax error: unclosed quote", 2);
	else if (err == TOK_SYNTAX_PIPE)
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
	else if (err == TOK_SYNTAX_REDIR)
		ft_putendl_fd("minishell: syntax error near unexpected token `<' or `>'", 2);
}

void	ft_expand_error(t_expand_error err)
{
	if (err == EXPAND_MEMORY_ALLOC)
		ft_putendl_fd("minishell: memory allocation error during expansion", 2);
}

void	ft_parser_error(t_parser_error err)
{
	if (err == PARSER_MEMORY_ALLOC)
		ft_putendl_fd("minishell: memory allocation error during parsing", 2);
	else if (err == PARSER_SYNTAX_PIPE)
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
	else if (err == PARSER_SYNTAX_REDIR)
		ft_putendl_fd("minishell: syntax error near unexpected token `<' or `>'", 2);
}

void	ft_executor_error(int err)
{
	(void)err;
}


