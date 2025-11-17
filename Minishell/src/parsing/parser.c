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

t_parser_error	parser_tokens(t_data *data)
{
	data->parser.tokens = data->tokenizer.tokens;
	init_parser(&data->parser, data->parser.tokens);
	return (process_tokens(data));
}
