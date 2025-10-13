/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:45:38 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/13 12:45:24 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void free_cmds(t_cmd *cmd)
{
	t_cmd *tmp;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->av)
		{
			int i = 0;
			while (cmd->av[i])
				free(cmd->av[i++]);
			free(cmd->av);
		}
		free(cmd->redir_in);
		free(cmd->redir_out);
		free(cmd);
		cmd = tmp;
	}
}


void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
}

void	ft_free_all(t_data *data)
{
	if (data->tokens)
	{
		free_tokens(data->tokens);
		data->tokens = NULL;
	}
	if (data->cmds_list)
	{
		free_cmds(data->cmds_list);
		data->cmds_list = NULL;
	}
	if (data->input_expanded)
	{
		free(data->input_expanded);
		data->input_expanded = NULL;
	}
}

void	free_array(char **array)
{
	int	i;

	if (array == NULL)
		return ;
	i = 0;
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
}
