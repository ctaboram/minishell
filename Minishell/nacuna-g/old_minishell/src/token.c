/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:13:14 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/26 09:52:33 by nacuna-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*create_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->value = ft_strdup(value);
	token->type = type;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **list, t_token *new_token)
{
	t_token	*tmp;

	if (*list == NULL)
		*list = new_token;
	else
	{
		tmp = *list;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new_token;
	}
}

static void	handle_quote(char **start, char **end, t_token **tokens, char quote)
{
	char	*value;

	*end = *end + 1;
	while (**end && **end != quote)
		*end = *end + 1;
	if (**end != quote)
	{
		printf("Error: unclosed quote\n");
		free_tokens(*tokens);
		*tokens = NULL;
		return ;
	}
	value = ft_strndup(*start + 1, *end - *start - 1);
	if (quote == '\'')
		add_token(tokens, create_token(value, TOKEN_QUOTE_SINGLE));
	else
		add_token(tokens, create_token(value, TOKEN_QUOTE_DOUBLE));
	free(value);
	*end = *end + 1;
	*start = *end;
}

static void	handle_special(char **start, char **end, t_token **tokens)
{
	if (**end == '|')
	{
		if (*(*end + 1) == '|')
		{
			add_token(tokens, create_token("||", TOKEN_OR));
			*start = *start + 2;
		}
		else
		{
			add_token(tokens, create_token("|", TOKEN_PIPE));
			*start = *start + 1;
		}
	}
	else if (**end == '>')
	{
		if (*(*end + 1) == '>')
		{
			add_token(tokens, create_token(">>", TOKEN_APPEND));
			*start = *start + 2;
		}
		else
		{
			add_token(tokens, create_token(">", TOKEN_REDIRECT_OUT));
			*start = *start + 1;
		}
	}
	else if (**end == '<')
	{
		if (*(*end + 1) == '<')
		{
			add_token(tokens, create_token("<<", TOKEN_HEREDOC));
			*start = *start + 2;
		}
		else
		{
			add_token(tokens, create_token("<", TOKEN_REDIRECT_IN));
			*start = *start + 1;
		}
	}
	else if (**end == '&' && *(*end + 1) == '&')
	{
		add_token(tokens, create_token("&&", TOKEN_AND));
		*start = *start + 2;
	}
	else if (**end == '(')
	{
		add_token(tokens, create_token("(", TOKEN_PAREN_OPEN));
		*start = *start + 1;
	}
	else if (**end == ')')
	{
		add_token(tokens, create_token(")", TOKEN_PAREN_CLOSE));
		*start = *start + 1;
	}
}

static void	handle_word(char **start, char **end, t_token **tokens)
{
	char	*value;

	while (**end && !ft_isspace(**end) && **end != '|' && **end != '>' && **end != '<'
		&& **end != '&' && **end != '(' && **end != ')')
		*end = *end + 1;
	value = ft_strndup(*start, *end - *start);
	add_token(tokens, create_token(value, TOKEN_WORD));
	free(value);
	*start = *end;
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
		if (*start == 0)
			break ;
		end = start;
		if (*end == '\'' || *end == '"')
			handle_quote(&start, &end, &tokens, *end);
		else if (*end == '|' || *end == '>' || *end == '<' || *end == '&'
			|| *end == '(' || *end == ')')
			handle_special(&start, &end, &tokens);
		else
			handle_word(&start, &end, &tokens);
		if (tokens == NULL)
			return (NULL);
	}
	add_token(&tokens, create_token("", TOKEN_EOF));
	return (tokens);
}


