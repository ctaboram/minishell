/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:01:11 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/12 10:42:08 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char *get_prompt(void)
{
	static char prompt[MAX_PROMPT_SIZE];
	char *username;

	username = getenv("USER");
	if(!username)
		username = getenv("LOGNAME");
	if(!username)
		username = "guest";
	snprintf(prompt,MAX_PROMPT_SIZE,BPURPLE"%s@minishell->"RESET,username);
	return(prompt);
}

int	ft_prompt(void)
{
	char	*line;
	t_token	*tokens;

	line = readline(get_prompt());
	if (!line)
	{
		printf("exit\n");
		return (0);
	}
	if (*line)
		add_history(line);
	if (ft_strcmp(line, "exit") == 0)
	{
		free(line);
		printf("exit\n");
		return (0);
	}
	else if (ft_strcmp(line, "") != 0)
	{
		tokens = tokenize(line);
		t_token *tmp = tokens;
		while (tmp)
		{
			printf("Token: %s, Type: %d\n", tmp->value, tmp->type);
			tmp = tmp->next;
		}
		while (tokens)
		{
			t_token *tmp = tokens;
			tokens = tokens->next;
			free(tmp->value);
			free(tmp);
		}
	}
	free(line);
	return (1);
}
