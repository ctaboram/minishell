/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_executor4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 15:20:00 by nikotina          #+#    #+#             */
/*   Updated: 2025/10/22 15:20:00 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	update_exit_status(t_data *data)
{
	if (g_signal_exit_code != 0)
	{
		data->exit_status = g_signal_exit_code;
		g_signal_exit_code = 0;
	}
	else
		data->exit_status = data->execute.exit_status;
}
