/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:55:49 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/16 13:08:04 by nacuna-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern char **environ;

char	**ft_copy_env(char **environ)
{
	char	**envp;
	int		len;
	int		i;

	len = 0;
	while (environ[len] != NULL)
		len++;
	envp = malloc(sizeof(char *) * (len + 1));
	if (envp == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		envp[i] = ft_strdup(environ[i]);
		i++;
	}
	envp[len] = NULL;
	return (envp);
}

int	main(void)
{
	char	**envp;

	envp = ft_copy_env(environ);
	handle_signals();
	printf(HEADER);
	while (ft_prompt(&envp))
		;
	ft_free_array(envp);
	return (g_exit_status);
}
