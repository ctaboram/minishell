/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:07:18 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/09 11:56:07 by nikotina         ###   ########.fr       */
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
	int status;

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
	if(ft_strcmp(data->input, ""))
	{
		status = tokenizer(data, data->tokenizer);
		if (status)
		{
			ft_tokenizer_error(status);	// FUNCION PARA FT_ERROR
									// 	if (status == ERR_SYNTAX_PIPE)
									// 		printf("minishell: syntax error near unexpected token `|'\n");
			return (PROMPT_CONTINUE);
		}
		status = expand_word(data);
		if (status)
		{
			ft_expand_error();
			return (PROMPT_CONTINUE);
		}
		status = parser_tokens(data);
		if (status)
		{
			ft_parser_error(status, data->tokens);
			return (PROMPT_CONTINUE);
		}
		status = executor();
		if (status)
		{
			ft_executor_error(status, data->tokens);
			return (PROMPT_CONTINUE);
		}
		ft_free_all();		// free_tokens(data->tokens);
							// data->tokens = NULL;
							// free_cmds(data->cmds);
							// data->cmds = NULL;
	}
	free(data->input);
	free(data->input_expanded);
	return (PROMPT_CONTINUE);
}
