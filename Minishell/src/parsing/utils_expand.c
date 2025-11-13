/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 10:29:25 by nikotina          #+#    #+#             */
/*   Updated: 2025/10/22 10:44:42 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_expand(t_data *data)
{
	data->expand.input = data->input;
	data->expand.env = data->env;
	data->expand.exit_status = data->exit_status;
	data->expand.result = ft_strdup("");
	data->expand.var = NULL;
	data->expand.value = NULL;
	data->expand.i = 0;
	data->expand.start = 0;
	data->expand.input_expanded = NULL;
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*tmp;

	tmp = ft_strjoin(s1, s2);
	free(s1);
	return (tmp);
}
