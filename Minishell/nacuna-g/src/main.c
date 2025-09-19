/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:55:49 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/19 09:36:09 by nacuna-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	*exit_status(void)
{
	static int	status;

	return (&status);
}

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

int	main(int argc, char **argv, char **env)
{
	char	**envp;

	(void)argc;
	(void)argv;
	envp = ft_copy_env(env);
	handle_signals();
	printf(HEADER);
	while (ft_prompt(&envp))
		;
	ft_free_array(envp);
	return (*exit_status());
}