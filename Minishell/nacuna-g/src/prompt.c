/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:01:11 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/16 11:15:59 by nacuna-g         ###   ########.fr       */
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
		username = "guest";
	snprintf(prompt, MAX_PROMPT_SIZE, BPURPLE"%s@minishell->"RESET, username);
	return (prompt);
}

int	ft_prompt(char ***envp)
{
	char	*line;
	t_token	*tokens;
	t_cmd	*cmds;

	line = readline(get_prompt());
	if (line == NULL)
	{
		printf("exit\n");
		return (0);
	}
	if (*line != 0)
		add_history(line);
	if (ft_strcmp(line, "exit") == 0)
	{
		free(line);
		printf("exit\n");
		return (0);
	}
	if (ft_strcmp(line, "") != 0)
	{
		tokens = tokenize(line);
		if (tokens != NULL)
		{
			cmds = parse(tokens);
			free_tokens(tokens);
			if (cmds != NULL)
			{
				expand_cmd(cmds, *envp);
				execute(cmds, envp);
				free_cmd(cmds);
			}
		}
	}
	free(line);
	return (1);
}
