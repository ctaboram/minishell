/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:01:11 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/09 11:01:25 by nacuna-g         ###   ########.fr       */
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

int ft_prompt(void)
{
	char *line;
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
		printf("Comando: %s\n", line);
	}
	if (ft_strcmp(line, "") != 0)
	{
		char **words;
		int i;

		i = 0;
		// SEPARO LA PALABRAS DEL PROMOPT Y LAS GURADO EN UN ARRAY
		words = ft_split(line, ' ');
		if (words && words[0])
		{
			// Comprobar si hay un segundo argumento y si empieza por '-'
			if (words[1] && words[1][0] == '-')
				printf("Comando detectado: %s, opción: %s\n", words[0], words[1]);
			else
				printf("Comando detectado: %s\n", words[0]);
			// Aquí puedes agregar la lógica para ejecutar el comando
		}
		else
		if (words)
		{
			while(words[i])
			{
				free(words[i]);
				i++;
			}
			free(words);
		}
	}
	free(line);
	return (1);
}
