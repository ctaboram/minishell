/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:12:49 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/16 12:13:02 by nacuna-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int builtin_echo(char **args)
{
	int i = 1;
	int n = 0;

	if (args[1] && ft_strcmp(args[1], "-n") == 0)
	{
		n = 1;
		i = 2;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i+1])
			printf(" ");
		i++;
	}
	if (n == 0)
		printf("\n");
	return (0);
}

static int builtin_cd(char **args, char ***envp)
{
	// Implementa cd, update PWD/OLDPWD in envp
	(void)envp;
	if (args[1] == NULL)
		chdir(getenv("HOME"));
	else if (chdir(args[1]) == -1)
		perror("cd");
	return (0);
}

// Añade otros: pwd, export, unset, env, exit

int is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "pwd") == 0 ||
		ft_strcmp(cmd, "export") == 0 || ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0 ||
		ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int exec_builtin(char **args, char ***envp)
{
	if (ft_strcmp(args[0], "echo") == 0)
		return (builtin_echo(args));
	else if (ft_strcmp(args[0], "cd") == 0)
		return (builtin_cd(args, envp));
	// Añade calls para otros
	return (1); // Error
}