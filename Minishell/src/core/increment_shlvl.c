/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   increment_shlvl.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 09:47:33 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/11/13 09:48:47 by nacuna-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	update_existing_shlvl(t_data *data, int i, int shlvl)
{
	char	*shlvl_str;
	char	*new_var;

	if (shlvl < 0)
		shlvl = 0;
	shlvl++;
	shlvl_str = ft_itoa(shlvl);
	if (!shlvl_str)
		fatal_error("Error allocating memory for SHLVL");
	new_var = ft_strjoin("SHLVL=", shlvl_str);
	free(shlvl_str);
	if (!new_var)
		fatal_error("Error allocating memory for SHLVL");
	free(data->env[i]);
	data->env[i] = new_var;
}

static void	add_initial_shlvl(t_data *data)
{
	char	*new_var;

	new_var = ft_strdup("SHLVL=1");
	if (!new_var)
		fatal_error("Error allocating memory for SHLVL");
	data->env = add_new_env(data->env, new_var);
	free(new_var);
}

void	increment_shlvl(t_data *data)
{
	int	i;
	int	shlvl;

	i = 0;
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], "SHLVL=", 6) == 0)
		{
			shlvl = ft_atoi(data->env[i] + 6);
			update_existing_shlvl(data, i, shlvl);
			return ;
		}
		i++;
	}
	add_initial_shlvl(data);
}
