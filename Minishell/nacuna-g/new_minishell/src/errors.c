/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 11:49:06 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/29 13:15:45 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_fatal_error(char *msg)
{
	ft_putendl_fd(msg, 1);
	exit(EXIT_FAILURE);
}

void ft_print_error(t_data *data, int error, int *exit_status)
{
	int	i;

	i = 0;
	if (error == ERR_MEMORY_ALLOC)
	{
		printf("Error: allocating memory\n");
		*exit_status = 1;
		return ;
	}
	else if (error == ERR_COPY_FAILED)
	{
		printf("Error: copying env\n");
		while (data->env[i])
			free(data->env[i++]);
		free(data->env);
		data->env = NULL;
		*exit_status = 1;
		return ;
	}
	else if (error == ERR_UNCLOSED_QUOTE)
	{
		printf("Error: unclosed quote\n");
		free_tokens(data->tokens);
		data->tokens = NULL;
		*exit_status = 2;
		return ;
	}
	
}
