/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:07:18 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/22 10:29:25 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_prompt(void)
{
	static char	prompt[MAX_PROMPT_SIZE];
	char		*username;
	char		cwd[512];

	username = getenv("USER");
	if (!username)
		username = getenv("LOGNAME");
	if (!username)
		username = "guest";
	if(!getcwd(cwd, sizeof(cwd)))
		cwd[0] = '\0';
	snprintf(prompt,MAX_PROMPT_SIZE,BPURPLE"%s@minishell~"YELLOW"%s->"RESET,username,cwd);
	return(prompt);
}

t_prompt_error	init_prompt(t_data *data)
{
	int	status;
	
	status = 0;
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
		status = expand_word(data);
		if (status)
		{
			data->exit_status = status;
			return (handler_error(data, ERROR_EXPAND));
		}
		status = tokenizer(data);
		if (status)
		{
			data->exit_status = status;
			return (handler_error(data, ERROR_TOKENIZER));
		}
		status = parser_tokens(data);
		if (status)
		{
			data->exit_status = status;
			return (handler_error(data, ERROR_PARSER));
		}
		status = execute(data);
		if (status)
			return (handler_error(data, ERROR_EXECUTOR));
		ft_free_all(data);
	}
	free(data->input);
	if (data->expand.input_expanded)
		free(data->expand.input_expanded);
	return (PROMPT_CONTINUE);
}
