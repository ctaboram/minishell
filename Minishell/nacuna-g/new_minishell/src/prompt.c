/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:07:18 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/29 15:28:30 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_prompt(void)
{
	static char	prompt[MAX_PROMPT_SIZE];
	char		*username;

	username = getenv("USER");
	if (username == NULL)
		username = getenv("LOGNAME");
	if (username == NULL)
		username = "Guess";
	snprintf(prompt, MAX_PROMPT_SIZE, BPURPLE"%s@minishell->"RESET, username);
	return (prompt);
}

int	init_prompt(t_data *data)
{
	data->input = readline(get_prompt());
	if (!data->input)
	{
		printf("exit\n");
		free(data->input);
		return (0);
	}
	if (*data->input != '\0')
		add_history(data->input);
	if (ft_strcmp(data->input, "exit") == 0)
	{
		printf("exit\n");
		free(data->input);
		return (0);
	}
	if(ft_strcmp(data->input, ""))
	{
		tokenizer(data, data->tokenizer);
		free_tokens(data->tokens);
		data->tokens = NULL;
	}
	free(data->input);
	return (1);
}
