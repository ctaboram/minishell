/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:45:38 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/09 11:46:46 by nikotina         ###   ########.fr       */
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
