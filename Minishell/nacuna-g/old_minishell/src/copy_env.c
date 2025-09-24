/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 12:28:42 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/23 12:29:00 by nacuna-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**ft_copy_env(char **env)
{
	char	**envp;
	int		len;

	len = 0;
	while (env[len])
		len++;
	envp = malloc(sizeof(char *) * (len + 1));
	if (envp == NULL)
		return (NULL);
	len = 0;
	while (env[len])
	{
		envp[len] = ft_strdup(env[len]);
		len++;
	}
	envp[len] = NULL;
	return (envp);
}
