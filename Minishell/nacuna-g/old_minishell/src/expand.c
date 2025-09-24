/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:12:49 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/24 12:00:44 by nacuna-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_var_value(char *var, char **envp)
{
	char	*value;
	int		i;

	i = 0;
	if (ft_strcmp(var, "?") == 0)
	{
		value = ft_itoa(*exit_status());
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

static char *expand_part(char *str, int *i, char **envp)
{
	char *var;
	char *tmp;
	int start;
	int var_len;

	start = *i + 1;
	*i = start;
	if (str[*i] == '\0' || (!ft_isalnum(str[*i]) && str[*i] != '_' && str[*i] != '?'))
	{
		*i = start - 1;
		return (ft_strdup(""));
	}
	while (ft_isalnum(str[*i]) || str[*i] == '_' || str[*i] == '?')
		*i = *i + 1;
	var_len = *i - start;
	var = ft_strndup(str + start, var_len);
	tmp = get_var_value(var, envp);
	free(var);
	return (tmp);
}

static char *copy_part(char *str, int *i)
{
	int start;
	char *tmp;

	start = *i;
	while (str[*i] && str[*i] != '$')
		*i = *i + 1;
	if (str[*i] == '$')
		tmp = ft_strndup(str + start, *i - start);
	else
		tmp = ft_strndup(str + start, *i - start);
	return (tmp);
}

char *expand_str(char *str, char **envp)
{
	char *result;
	char *tmp;
	int i;
	int start;

	if (!str || !ft_strchr(str, '$'))
		return (ft_strdup(str));
	result = ft_strdup("");
	if (result == NULL)
		return (NULL);
	i = 0;
	while (str[i] != '\0')
	{
		start = i;
		if (str[i] == '$')
		{
			tmp = expand_part(str, &i, envp);
			if (tmp != NULL)
			{
				result = ft_strjoin(result, tmp);
				free(tmp);
			}
			if (i == start + 1)
			{
				tmp = ft_strdup("$");
				if (tmp != NULL)
				{
					result = ft_strjoin(result, tmp);
					free(tmp);
				}
			}
		}
		else
		{
			tmp = copy_part(str, &i);
			if (tmp != NULL)
			{
				result = ft_strjoin(result, tmp);
				free(tmp);
			}
		}
	}
	return (result);
}

static void expand_args(char **args, char **envp)
{
    int     i;
    char    *expanded;

    i = 0;
    while (args[i] != NULL)
    {
        if (ft_strchr(args[i], '$') == NULL)
        {
            i++;
            continue ;
        }
        expanded = expand_str(args[i], envp);
        free(args[i]);
        if (expanded == NULL)
            expanded = ft_strdup("");  // Fallback para evitar NULL
        args[i] = expanded;
        i++;
    }
}

static void expand_redirs(t_redir *redir, char **envp)
{
    char    *expanded;

    while (redir != NULL)
    {
        if (redir->type != TOKEN_HEREDOC)
        {
            expanded = expand_str(redir->file, envp);
            free(redir->file);
            if (expanded == NULL)
                expanded = ft_strdup("");  // Evita NULL en file
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
