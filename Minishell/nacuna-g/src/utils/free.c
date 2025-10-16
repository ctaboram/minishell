/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:45:38 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/15 11:46:05 by nikotina         ###   ########.fr       */
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
	// Liberar tokens del tokenizer
	if (data->tokenizer.tokens)
		free_tokens(data->tokenizer.tokens);
	// Liberar comandos del parser
	if (data->parser.cmds_list)
		free_cmds(data->parser.cmds_list);
	// Liberar input expandido
	if (data->expand.input_expanded)
		free(data->expand.input_expanded);
	
	// Resetear punteros
	data->tokenizer.tokens = NULL;
	data->parser.cmds_list = NULL;
	data->expand.input_expanded = NULL;
	data->execute.cmds_list = NULL;
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
