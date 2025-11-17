/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokenizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:05:29 by nikotina          #+#    #+#             */
/*   Updated: 2025/09/29 14:05:53 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*create_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	if (value)
		token->value = ft_strdup(value);
	else
		token->value = NULL;
	if (value && !token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

void	add_token(t_token **tokens, t_token *token)
{
	t_token	*current;

	if (!token)
		return ;
	if (!*tokens)
	{
		*tokens = token;
		token->next = NULL;
		token->prev = NULL;
		return ;
	}
	else
	{
		current = *tokens;
		while (current->next)
			current = current->next;
		current->next = token;
		token->prev = current;
		token->next = NULL;
	}
}
