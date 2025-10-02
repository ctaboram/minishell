/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 12:29:46 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/02 09:51:03 by nacuna-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd	*new_cmd(void)
{
	t_cmd *cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->av = NULL;
	cmd->redir_in = NULL;
	cmd->redir_out = NULL;
	cmd->append = 0;
	cmd->next = NULL;
	return cmd;
}

void	append_arg(t_cmd *cmd, char *value)
{
	int		i;
	int		j;
	char	**new_av;

	i = 0;
	j = -1;
	while (cmd->av && cmd->av[i])
		i++;
	new_av = malloc(sizeof(char *) * (i + 2));
	if (!new_av)
		return ;
	while (++j < i)
		new_av[j] = cmd->av[j];
	new_av[i] = ft_strdup(value);
	new_av[i + 1] = NULL;
	free (cmd->av);
	cmd->av = new_av;
}
