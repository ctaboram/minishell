/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctaboada <ctaboada@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:01:11 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/16 12:13:35 by ctaboada         ###   ########.fr       */
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

	int ft_prompt(char **env,char **dup_env)
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
	// Elimina los prints de depuración y solo ejecuta la lógica
	if (ft_strcmp(line, "") != 0)
	{
		char **words;
		int i;
		i = 0;
		// SEPARO LAS PALABRAS DEL PROMPT Y LAS GUARDO EN UN ARRAY
		words = ft_split(line, ' ');
		if (words && words[0])
		{
			if (ft_strcmp(words[0], "cd") == 0)
				ft_builtin_cd(words);
			else if (ft_strcmp(words[0], "echo") == 0)
				ft_builting_echo(words);
			else if (ft_strcmp(words[0], "pwd") == 0)
				ft_builtin_pwd();
			else if(ft_strcmp(words[0], "env") == 0)
				ft_builtin_env(env);
			else if(ft_strcmp(words[0],"export") == 0)
				ft_builtin_export(words,dup_env);
			// Aquí puedes agregar la lógica para ejecutar el comando real
			// Por ahora, no mostramos nada en pantalla
		}
		// Liberar memoria
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
