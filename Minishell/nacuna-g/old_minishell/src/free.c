/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 09:31:26 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/23 09:49:01 by nacuna-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_free_array(char **array)
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

static void	free_redirs(t_redir *redir)
{
	t_redir	*tmp;

	while (redir != NULL)
	{
		tmp = redir;
		redir = redir->next;
		free(tmp->file);
		free(tmp);
	}
}

void	free_cmd(t_cmd *cmds)
{
	t_cmd	*tmp_cmd;
	int		i;

	while (cmds != NULL)
	{
		tmp_cmd = cmds;
		cmds = cmds->next;
		i = 0;
		if (tmp_cmd->args != NULL)
		{
			while (tmp_cmd->args[i] != NULL)
			{
				free(tmp_cmd->args[i]);
				i++;
			}
			free(tmp_cmd->args);
		}
		free_redirs(tmp_cmd->redir_in);
		free_redirs(tmp_cmd->redir_out);
		free(tmp_cmd);
	}
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}
