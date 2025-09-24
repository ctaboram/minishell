/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:55:49 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/23 13:07:06 by nacuna-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	*exit_status(void)
{
	static int	status;

	return (&status);
}

int	main(int argc, char **argv, char **env)
{
	char	**envp;

	(void)argc;
	(void)argv;
	envp = ft_copy_env(env);
	handle_signals();
	printf(HEADER);
	while (1)
	{
		envp = ft_prompt(envp);
		if (envp == NULL)
			break;
	}
	ft_free_array(envp);
	return (*exit_status());
}