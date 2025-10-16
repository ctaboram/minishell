/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:05:29 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/09 11:46:44 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **env)
{
	t_data	data;

	(void)ac;
	(void)av;
	printf(HEADER);
	init_data(&data, env);
	setup_signals();
	while (1)
	{
		if(init_prompt(&data))
			break ;
	}
	// Las estructuras ahora son parte de data, no necesitan free separado
	free_array(data.env);
	return (0);
}
