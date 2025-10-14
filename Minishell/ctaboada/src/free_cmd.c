/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctaboada <ctaboada@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 11:31:54 by ctaboada          #+#    #+#             */
/*   Updated: 2025/10/14 11:32:51 by ctaboada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void free_cmds(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		i;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->av)
		{
			i = 0;
			while (cmd->av[i])
				free(cmd->av[i++]);
			free(cmd->av);
		}

		if (cmd->redir_in)
			free(cmd->redir_in);
		if (cmd->redir_out)
			free(cmd->redir_out);

		free(cmd);

		cmd = tmp;
	}
}