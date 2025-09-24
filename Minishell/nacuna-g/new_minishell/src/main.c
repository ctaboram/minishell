/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:05:29 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/24 13:18:56 by nacuna-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **env)
{
	t_data *data;

	(void)ac;
	(void)av;
	printf(HEADER);
	data = init_data();
	cpy_env(data, env);
	init_prompt(data);
	
	return (0);
}
