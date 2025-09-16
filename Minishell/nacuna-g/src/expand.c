/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:12:49 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/16 12:11:29 by nacuna-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_exit_status = 0;

static char	*get_var_value(char *var, char **envp)
{
	char	*value;
	int		i;

	i = 0;
	if (ft_strcmp(var, "?") == 0)
	{
		value = ft_itoa(g_exit_status);
		return (value);
	}
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], var, ft_strlen(var)) == 0 && envp[i][ft_strlen(var)] == '=')
			return (ft_strdup(envp[i] + ft_strlen(var) + 1));
		i++;
	}
	return (ft_strdup(""));
}

static char	*expand_part(char *str, int *i, char **envp)
{
	char	*var;
	char	*tmp;
	int		start;

	start = *i + 1;
	*i = start;
	while (ft_isalnum(str[*i]) || str[*i] == '_' || str[*i] == '?')
		*i = *i + 1;
	var = ft_strndup(str + start, *i - start);
	tmp = get_var_value(var, envp);
	free(var);
	return (tmp);
}

static char	*copy_part(char *str, int *i)
{
	int		start;
	char	*tmp;

	start = *i;
	while (str[*i] && str[*i] != '$')
		*i = *i + 1;
	tmp = ft_strndup(str + start, *i - start);
	return (tmp);
}

char	*expand_str(char *str, char **envp)
{
	char	*result;
	char	*tmp;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (str[i] != 0)
	{
		if (str[i] == '$')
		{
			tmp = expand_part(str, &i, envp);
			result = ft_strjoin_free(result, tmp);
			free(tmp);
		}
		else
		{
			tmp = copy_part(str, &i);
			result = ft_strjoin_free(result, tmp);
			free(tmp);
		}
	}
	return (result);
}
static void	expand_args(char **args, char **envp)
{
	int		i;
	char	*expanded;

	i = 0;
	while (args[i] != NULL)
	{
		if (ft_strchr(args[i], '$') == NULL)
			i++;
		else
		{
			expanded = expand_str(args[i], envp);
			free(args[i]);
			args[i] = expanded;
			i++;
		}
	}
}

static void	expand_redirs(t_redir *redir, char **envp)
{
	char	*expanded;

	while (redir != NULL)
	{
		if (redir->type != TOKEN_HEREDOC)
		{
			expanded = expand_str(redir->file, envp);
			free(redir->file);
			redir->file = expanded;
		}
		redir = redir->next;
	}
}

void	expand_cmd(t_cmd *cmds, char **envp)
{
	t_cmd	*cmd;

	cmd = cmds;
	while (cmd != NULL)
	{
		expand_args(cmd->args, envp);
		expand_redirs(cmd->redir_in, envp);
		expand_redirs(cmd->redir_out, envp);
		cmd = cmd->next;
	}
}
