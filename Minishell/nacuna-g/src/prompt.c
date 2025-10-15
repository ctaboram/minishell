/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:07:18 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/15 11:31:08 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char *get_prompt(void)
	{
	static char	prompt[MAX_PROMPT_SIZE];
	char		*username;
	char		cwd[512];

	username = getenv("USER");
	if(!username)
		username = getenv("LOGNAME");
	if(!username)
		username = "guest";
	if(!getcwd(cwd, sizeof(cwd)))
		cwd[0] = '\0';
	snprintf(prompt,MAX_PROMPT_SIZE,BPURPLE"%s@minishell~"YELLOW"%s->"RESET,username,cwd);
	return(prompt);
}

t_prompt_error	init_prompt(t_data *data)
{
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
		t_expand_error expand_err = expand_word(data);
		if (expand_err)
		{
			data->exit_status = expand_err;
			return (handler_error(data, ERROR_EXPAND));
		}
		t_tokenizer_error tok_err = tokenizer(data, data->tokenizer);
		if (tok_err)
		{
			data->exit_status = tok_err;
			return (handler_error(data, ERROR_TOKENIZER));
		}
		t_parser_error parse_err = parser_tokens(data);
		if (parse_err)
		{
			data->exit_status = parse_err;
			return (handler_error(data, ERROR_PARSER));
		}
		t_executor_error exec_err = execute(data, data->cmds_list);
		if (exec_err)
		{
			// Note: exit_status is already set by execute() function
			return (handler_error(data, ERROR_EXECUTOR));
		}
		ft_free_all(data);
	}
	free(data->input);
	free(data->input_expanded);
	return (PROMPT_CONTINUE);
}
