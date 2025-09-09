/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:55:49 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/08 13:04:54 by nacuna-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

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

/**
 * ft_prompt - Muestra un prompt interactivo y procesa comandos básicos.
 * Retorna 0 al salir (comando "exit"), 1 para continuar el bucle.
 */
int ft_prompt(void)
{
	char *line;

	// Mostrar el prompt y leer la entrada del usuario
	line = readline(get_prompt());
	if (!line) // Si readline retorna NULL (EOF, Ctrl+D)
	{
		printf("exit\n");
		return (0); // Terminar el programa
	}

	// Si la línea no está vacía, añadirla al historial
	if (*line)
		add_history(line);

	// Procesar comandos básicos
	if (ft_strcmp(line, "exit") == 0)
	{
		free(line);
		printf("exit\n");
		return (0); // Salir del programa
	}
	else if (ft_strcmp(line, "") != 0) // Si no es una línea vacía
	{
		printf("Comando: %s\n", line); // Simulación de ejecución de comando
		// Aquí iría la lógica para parsear y ejecutar comandos
	}

	free(line); // Liberar la memoria de la línea leída
	return (1); // Continuar el bucle
}

/**
 * main - Punto de entrada para probar el prompt.
 */
int main(void)
{
	while (ft_prompt())
		;
	return (0);
}
