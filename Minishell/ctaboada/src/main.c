/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctaboada <ctaboada@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:55:49 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/06 12:04:21 by ctaboada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	t_data data;  // Usamos la estructura t_data

	data.env = dup_env(env);  // Inicializamos el env
	data.input = NULL;        // Inicializamos otros campos
	data.tokens = NULL;
	data.exit_status = 0;

	printf(HEADER);
	while (ft_prompt(&data))  // Pasamos &data
		;

	// Liberamos recursos al final
	ft_free_env(data.env);
	// Si hay tokens o input pendientes, libéralos aquí (por ahora no)
	return (data.exit_status);
}
