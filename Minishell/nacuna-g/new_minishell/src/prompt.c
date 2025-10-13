/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:07:18 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/13 12:59:17 by nikotina         ###   ########.fr       */
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

t_prompt_error	init_prompt(t_data *data)
{
	data->exit_status = 0;
	data->input = readline(get_prompt());
	if (!data->input)
	{
		printf("exit\n");
		free(data->input);
		return (PROMPT_EOF);
	}
	if (*data->input != '\0')
		add_history(data->input);
	if (ft_strcmp(data->input, "exit") == 0)
	{
		printf("exit\n");
		free(data->input);
		return (PROMPT_EXIT);
	}
	if (ft_strcmp(data->input, ""))
	{
		data->exit_status = tokenizer(data, data->tokenizer);
		if (data->exit_status)
			return (handler_error(data, ERROR_TOKENIZER));
		data->exit_status = expand_word(data);
		if (data->exit_status)
			return (handler_error(data, ERROR_EXPAND));
		data->exit_status = parser_tokens(data);
		if (data->exit_status)
			return (handler_error(data, ERROR_PARSER));
		data->exit_status = executor();
		if (data->exit_status)
			return (handler_error(data, ERROR_EXECUTOR));
		ft_free_all(data);
	}
	free(data->input);
	free(data->input_expanded);
	return (PROMPT_CONTINUE);
}
