/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:42:58 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/29 15:36:38 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"\

static void	cpy_env(t_data *data, char **env)
{
	int i;

	i = 0;
	while (env[i])
		i++;
	data->env = malloc(sizeof(char *) * (i + 1));
	if (!data->env)
		ft_fatal_error("Error allocating memory for env");
	i = 0;
	while (env[i])
	{
		data->env[i] = ft_strdup(env[i]);
		if (!data->env[i])
			ft_fatal_error("Error duplicating env string");
		i++;
	}
	data->env[i] = NULL;
}

void	init_data(t_data *data, char **env)
{
	data->tokens = NULL;
	data->exit_status = 0;
	data->tokenizer->start = NULL;
	data->tokenizer->end = NULL;
	data->env = NULL;
	cpy_env(data, env);
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
