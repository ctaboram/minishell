/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:12:49 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/18 12:32:44 by nacuna-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int is_builtin(char *cmd)
{
	if (cmd == NULL)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0 || 
		ft_strcmp(cmd, "cd") == 0 || 
		ft_strcmp(cmd, "pwd") == 0 ||
		ft_strcmp(cmd, "export") == 0 || 
		ft_strcmp(cmd, "unset") == 0 || 
		ft_strcmp(cmd, "env") == 0 ||
		ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

static int builtin_echo(char **args)
{
	int i;
	int n;

	i = 1;
	n = 0;
	if (args[1] == NULL)
	{
		printf("\n");
		return (0);
	}
	while (args[i] && ft_strcmp(args[i], "-n") == 0)
	{
		n = 1;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (n == 0)
		printf("\n");
	return (0);
}

static void update_env_var(char ***envp, char *var, char *value)
{
	char *new_var;
	char *temp;
	size_t i;
	size_t var_len;
	size_t env_size;

	if (value == NULL || *value == '\0')
		return ;
	var_len = ft_strlen(var);
	env_size = 0;
	while ((*envp)[env_size] != NULL)
		env_size++;
	i = 0;
	while ((*envp)[i] != NULL)
	{
		if (ft_strncmp((*envp)[i], var, var_len) == 0 && 
			(*envp)[i][var_len] == '=')
		{
			free((*envp)[i]);
			temp = ft_strjoin(var, "=");
			if (temp == NULL)
				return ;
			new_var = ft_strjoin(temp, value);
			free(temp);
			if (new_var != NULL)
				(*envp)[i] = new_var;
			return ;
		}
		i++;
	}
	char **new_envp;

	new_envp = calloc(env_size + 2, sizeof(char *));
	if (new_envp == NULL)
	{
		perror("calloc envp");
		return ;
	}
	i = 0;
	while (i < env_size)
	{
		new_envp[i] = ft_strdup((*envp)[i]);
		if (new_envp[i] == NULL)
		{
			while (i > 0)
				free(new_envp[--i]);
			free(new_envp);
			perror("calloc strdup");
			return ;
		}
		i++;
	}
	temp = ft_strjoin(var, "=");
	if (temp == NULL)
	{
		while (i > 0)
			free(new_envp[--i]);
		free(new_envp);
		return ;
	}
	new_var = ft_strjoin(temp, value);
	free(temp);
	if (new_var == NULL)
	{
		while (i > 0)
			free(new_envp[--i]);
		free(new_envp);
		return ;
	}
	new_envp[i] = new_var;
	i = 0;
	while ((*envp)[i] != NULL)
	{
		free((*envp)[i]);
		i++;
	}
	free(*envp);
	*envp = new_envp;
}

static int builtin_cd(char **args, char ***envp)
{
	char old_cwd[1024];
	char new_cwd[1024];
	char *old_pwd;
	char *new_pwd;
	char *target;

	old_pwd = NULL;
	new_pwd = NULL;
	if (getcwd(old_cwd, sizeof(old_cwd)) != NULL)
		old_pwd = ft_strdup(old_cwd);
	else
		old_pwd = ft_strdup(getenv("PWD") ? getenv("PWD") : ".");
	if (old_pwd == NULL)
	{
		perror("cd: getcwd");
		return (1);
	}
	if (args[1] == NULL)
	{
		target = getenv("HOME");
		if (target == NULL)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			free(old_pwd);
			return (1);
		}
	}
	else
		target = args[1];
	if (chdir(target) == -1)
	{
		perror("minishell: cd");
		free(old_pwd);
		return (1);
	}
	if (getcwd(new_cwd, sizeof(new_cwd)) != NULL)
		new_pwd = ft_strdup(new_cwd);
	else
	{
		if (target[0] == '/')
			new_pwd = ft_strdup(target);
		else
		{
			char *temp;

			temp = ft_strjoin(old_pwd, "/");
			new_pwd = ft_strjoin(temp, target);
			free(temp);
		}
	}
	if (new_pwd == NULL)
	{
		perror("cd: getcwd");
		free(old_pwd);
		return (1);
	}
	update_env_var(envp, "OLDPWD", old_pwd);
	update_env_var(envp, "PWD", new_pwd);
	free(old_pwd);
	free(new_pwd);
	return (0);
}

static int builtin_pwd(char **args, char ***envp)
{
	char cwd[1024];

	(void)args;
	(void)envp;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", cwd);
	return (0);
}

static int builtin_env(char **args, char ***envp)
{
	int i;

	(void)args;
	i = 0;
	while ((*envp)[i] != NULL)
	{
		if (ft_strchr((*envp)[i], '=') != NULL)
			printf("%s\n", (*envp)[i]);
		i++;
	}
	return (0);
}

static int builtin_exit(char **args, char ***envp)
{
	int status;

	(void)envp;
	printf("exit\n");
	if (args[1] == NULL)
		exit(0);
	if (ft_strcmp(args[1], "-") == 0 || !ft_isdigit(args[1][0]))
		exit(255);
	status = ft_atoi(args[1]);
	exit(status & 0xFF);
	return (0);  // No alcanza, pero por norma
}

int exec_builtin(char **args, char ***envp)
{
	if (args == NULL || args[0] == NULL)
		return (1);
	if (ft_strcmp(args[0], "echo") == 0)
		return (builtin_echo(args));
	else if (ft_strcmp(args[0], "cd") == 0)
		return (builtin_cd(args, envp));
	else if (ft_strcmp(args[0], "pwd") == 0)
		return (builtin_pwd(args, envp));
	else if (ft_strcmp(args[0], "env") == 0)
		return (builtin_env(args, envp));
	else if (ft_strcmp(args[0], "exit") == 0)
		return (builtin_exit(args, envp));
	return (1);
}