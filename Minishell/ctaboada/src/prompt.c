/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctaboada <ctaboada@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:01:11 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/14 11:19:43 by ctaboada         ###   ########.fr       */
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

int	ft_prompt(t_data *data)
{
	char	*line;

	line = readline(get_prompt());
	if (!line)
	{
		printf("exit\n");
		data->exit_status = 0;
		return (0);
	}
	if (*line)
		add_history(line);
	data->input = ft_strdup(line);
	free(line);
	if (!data->input)
		return (0);
	if (ft_strcmp(data->input, "exit") == 0)
	{
		printf("exit\n");
		free(data->input);
		data->input = NULL;
		data->exit_status = 0;
		return (0);
	}
	return (1);
}
