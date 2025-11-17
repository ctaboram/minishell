/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 12:29:46 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/10 10:25:38 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_parser(t_parser *parser, t_token *tokens)
{
	parser->current = tokens;
	parser->head = new_cmd();
	parser->cmd = parser->head;
}

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->av = NULL;
	cmd->redir_in = NULL;
	cmd->redir_out = NULL;
	cmd->is_append = 0;
	cmd->is_heredoc = 0;
	cmd->heredoc_delim = NULL;
	cmd->next = NULL;
	return (cmd);
}

t_parser_error	append_arg(t_cmd *cmd, char *value)
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
		return (PARSER_MEMORY_ALLOC);
	while (++j < i)
		new_av[j] = cmd->av[j];
	new_av[i] = ft_strdup(value);
	if (!new_av[i])
	{
		free(new_av);
		return (PARSER_MEMORY_ALLOC);
	}
	new_av[i + 1] = NULL;
	free (cmd->av);
	cmd->av = new_av;
	return (PARSER_OK);
}

int	handle_heredoc(t_cmd *cmd, char *delimiter)
{
	char	*tmp_file;
	int		fd;
	int		saved_exit_code;

	saved_exit_code = g_signal_exit_code;
	g_signal_exit_code = 0;
	setup_heredoc_signals();
	fd = open_heredoc_file(&tmp_file, saved_exit_code);
	if (fd == -1)
		return (-1);
	if (read_heredoc_lines(fd, delimiter, tmp_file) == -1)
	{
		setup_signals();
		return (-1);
	}
	setup_signals();
	g_signal_exit_code = saved_exit_code;
	cmd->redir_in = tmp_file;
	cmd->is_heredoc = 1;
	return (0);
}
