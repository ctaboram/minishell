/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:27:06 by nikotina          #+#    #+#             */
/*   Updated: 2025/09/12 10:45:53 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*create_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	token->type = type;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **list, t_token *new_token)
{
	t_token	*tmp;

	if (!*list)
		*list = new_token;
	else
	{
		tmp = *list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_token;
	}
}

t_token	*tokenize(char *line)
{
	t_token	*tokens;
	char	*start;
	char	*end;

	tokens = NULL;
	start = line;
	while (*start)
	{
		while (*start && ft_isspace(*start))
			start++;
		if (!*start)
			break;
		end = start;
		if (*end == '|')
		{
			add_token(&tokens, create_token("|", TOKEN_PIPE));
			start++;
		}
		else if (*end == '>' && *(end + 1) == '>')
		{
			add_token(&tokens, create_token(">>", TOKEN_APPEND));
			start += 2;
		}
		else if (*end == '>')
		{
			add_token(&tokens, create_token(">", TOKEN_REDIRECT_OUT));
			start++;
		}
		else if (*end == '<')
		{
			add_token(&tokens, create_token("<", TOKEN_REDIRECT_IN));
			start++;
		}
		else if (*end == '\'' || *end == '"')
		{
			char quote = *end;
			end++;
			while (*end && *end != quote)
				end++;
			if (*end == quote)
			{
				char *value = ft_strndup(start + 1, end - start - 1);
				add_token(&tokens, create_token(value, TOKEN_QUOTE));
				free(value);
				end++;
			}
			start = end;
		}
		else
		{
			while (*end && !ft_isspace(*end) && *end != '|' && *end != '>' && *end != '<')
				end++;
			char *value = ft_strndup(start, end - start);
			add_token(&tokens, create_token(value, TOKEN_WORD));
			free(value);
			start = end;
		}
	}
	add_token(&tokens, create_token("", TOKEN_EOF));
	return (tokens);
}
