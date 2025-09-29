/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:05:29 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/29 13:24:05 by nikotina         ###   ########.fr       */
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
	while (1)
	{
		if(!init_prompt(&data))
			break ;
	}
	ft_free_array(data.env);
	return (0);
}
