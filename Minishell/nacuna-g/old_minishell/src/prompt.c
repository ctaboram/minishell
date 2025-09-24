/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:01:11 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/24 13:22:25 by nacuna-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static char	*get_prompt(void)
// {
// 	static char	prompt[MAX_PROMPT_SIZE];
// 	char		*username;

// 	username = getenv("USER");
// 	if (username == NULL)
// 		username = getenv("LOGNAME");
// 	if (username == NULL)
// 		username = "guest";
// 	snprintf(prompt, MAX_PROMPT_SIZE, BPURPLE"%s@minishell->"RESET, username);
// 	return (prompt);
// }

char	**ft_prompt(char **envp)
{
	char	*line;
	t_token	*tokens;
	t_cmd	*cmds;

	line = readline(get_prompt());
	if (line == NULL)
	{
		printf("exit\n");
		free(line);
		return (NULL);
	}
	if (*line != '\0')
		add_history(line);
	if (ft_strcmp(line, "") != 0)
	{
		tokens = tokenize(line);
		if (tokens != NULL)
		{
			cmds = parse(tokens);
			free_tokens(tokens);
			if (cmds != NULL)
			{
				expand_cmd(cmds, envp);
				envp = execute(cmds, envp);
				free_cmd(cmds);
			}
		}
	}
	free(line);
	return (envp);
}
