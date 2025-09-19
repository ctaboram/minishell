/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:12:59 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/19 12:39:00 by nacuna-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (cmd == NULL)
		return (NULL);
	cmd->args = NULL;
	cmd->redir_in = NULL;
	cmd->redir_out = NULL;
	cmd->next = NULL;
	return (cmd);
}

static t_redir	*new_redir(char *file, t_token_type type)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (redir == NULL)
		return (NULL);
	redir->file = ft_strdup(file);
	redir->type = type;
	redir->next = NULL;
	return (redir);
}

static void	add_redir(t_redir **list, t_redir *new)
{
	t_redir	*tmp;

	if (*list == NULL)
		*list = new;
	else
	{
		tmp = *list;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new;
	}
}

static void	add_arg(char ***args, char *value)
{
	int		len;
	char	**new_args;

	len = 0;
	if (*args != NULL)
	{
		while ((*args)[len] != NULL)
			len++;
	}
	new_args = malloc(sizeof(char *) * (len + 2));
	if (new_args == NULL)
		return ;
	len = 0;
	while (*args != NULL && (*args)[len] != NULL)
	{
		new_args[len] = (*args)[len];
		len++;
	}
	new_args[len] = ft_strdup(value);
	new_args[len + 1] = NULL;
	free(*args);
	*args = new_args;
	return ;
}

static int	handle_redir(t_token **tmp, t_cmd *current_cmd)
{
	t_token_type	type;

	type = (*tmp)->type;
	*tmp = (*tmp)->next;
	if (*tmp == NULL || (*tmp)->type != TOKEN_WORD)
	{
		printf("syntax error near unexpected token\n");
		return (1);
	}
	if (type == TOKEN_REDIRECT_IN || type == TOKEN_HEREDOC)
		add_redir(&current_cmd->redir_in, new_redir((*tmp)->value, type));
	else
		add_redir(&current_cmd->redir_out, new_redir((*tmp)->value, type));
	return (0);
}

t_cmd *parse(t_token *tokens)
{
	t_cmd	*cmds;
	t_cmd	*current_cmd;
	t_token	*tmp;

	cmds = NULL;
	current_cmd = NULL;
	tmp = tokens;
	while (tmp != NULL && tmp->type != TOKEN_EOF)
	{
		if (current_cmd == NULL)
		{
			current_cmd = new_cmd();
			if (current_cmd == NULL)
			{
				free_cmd(cmds);
				return (NULL);
			}
			if (cmds == NULL)
				cmds = current_cmd;
			else
			{
				t_cmd *last;

				last = cmds;
				while (last->next != NULL)
					last = last->next;
				last->next = current_cmd;
			}
		}
		if (tmp->type == TOKEN_WORD || tmp->type == TOKEN_QUOTE_SINGLE || 
			tmp->type == TOKEN_QUOTE_DOUBLE)
			add_arg(&current_cmd->args, tmp->value);
		else if (tmp->type == TOKEN_REDIRECT_IN || tmp->type == TOKEN_HEREDOC ||
				 tmp->type == TOKEN_REDIRECT_OUT || tmp->type == TOKEN_APPEND)
		{
			if (handle_redir(&tmp, current_cmd))
			{
				free_cmd(cmds);
				return (NULL);
			}
		}
		else if (tmp->type == TOKEN_PIPE)
		{
			if (current_cmd->args == NULL)
			{
				printf("syntax error near unexpected token `|'\n");
				free_cmd(cmds);
				return (NULL);
			}
			current_cmd = NULL;
		}
		tmp = tmp->next;
	}
	if (current_cmd != NULL && current_cmd->args == NULL)
	{
		printf("syntax error: incomplete command\n");
		free_cmd(cmds);
		return (NULL);
	}
	return (cmds);
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
