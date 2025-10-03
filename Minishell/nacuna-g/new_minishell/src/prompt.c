/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:07:18 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/03 11:28:55 by nacuna-g         ###   ########.fr       */
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
			ft_errors();	// FUNCION PARA FT_ERROR
							// 	if (status == ERR_SYNTAX_PIPE)
							// 		printf("minishell: syntax error near unexpected token `|'\n");
		status = parser_tokens(data);
		if (status)
			ft_errors();
		ft_free_all();		// free_tokens(data->tokens);
							// data->tokens = NULL;
							// free_cmds(data->cmds);
							// data->cmds = NULL;
	}
	free(data->input);
	return (PROMPT_CONTINUE);
}
