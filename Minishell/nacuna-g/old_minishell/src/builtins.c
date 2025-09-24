/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:12:49 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/24 09:34:38 by nacuna-g         ###   ########.fr       */
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

static char	**builtin_echo(char **args)
{
	int	i;
	int	n;

	i = 1;
	n = 0;
	if (args[1] == NULL)
	{
		printf("\n");
		return (NULL);
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
	return (NULL);
}

static char	**update_env_var(char **envp, char *var, char *value)
{
	char	*new_var;
	char	*temp;
	size_t	i;
	size_t	var_len;
	size_t	env_size;
	char	**new_envp;

	if (value == NULL || *value == '\0')
		return (envp);
	var_len = ft_strlen(var);
	env_size = 0;
	while (envp[env_size] != NULL)
		env_size++;
	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], var, var_len) == 0 && 
			envp[i][var_len] == '=')
		{
			free(envp[i]);
			temp = ft_strjoin(var, "=");
			if (temp == NULL)
				return (envp);
			new_var = ft_strjoin(temp, value);
			free(temp);
			if (new_var != NULL)
				envp[i] = new_var;
			return (envp);
		}
		i++;
	}
	new_envp = calloc(env_size + 2, sizeof(char *));
	if (new_envp == NULL)
	{
		perror("calloc envp");
		return (envp);
	}
	i = 0;
	while (i < env_size)
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (new_envp[i] == NULL)
		{
			while (i > 0)
				free(new_envp[--i]);
			free(new_envp);
			perror("calloc strdup");
			return (envp);
		}
		i++;
	}
	temp = ft_strjoin(var, "=");
	if (temp == NULL)
	{
		while (i > 0)
			free(new_envp[--i]);
		free(new_envp);
		return (envp);
	}
	new_var = ft_strjoin(temp, value);
	free(temp);
	if (new_var == NULL)
	{
		while (i > 0)
			free(new_envp[--i]);
		free(new_envp);
		return (envp);
	}
	new_envp[i] = new_var;
	ft_free_array(envp);
	return (new_envp);
}

static char	**builtin_cd(char **args, char **envp)
{
	char	old_cwd[1024];
	char	new_cwd[1024];
	char	*old_pwd;
	char	*new_pwd;
	char	*target;

	old_pwd = NULL;
	new_pwd = NULL;
	if (getcwd(old_cwd, sizeof(old_cwd)) != NULL)
		old_pwd = ft_strdup(old_cwd);
	else
	{
		char *pwd_env = getenv("PWD");
		if (pwd_env != NULL)
			old_pwd = ft_strdup(pwd_env);
		else
			old_pwd = ft_strdup(".");
	}
	if (old_pwd == NULL)
	{
		perror("cd: getcwd");
		return (envp);
	}
	if (args[1] == NULL)
	{
		char *home_env = getenv("HOME");
		if (home_env == NULL)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			free(old_pwd);
			return (envp);
		}
		target = home_env;
	}
	else
		target = args[1];
	if (chdir(target) == -1)
	{
		perror("minishell: cd");
		free(old_pwd);
		return (envp);
	}
	if (getcwd(new_cwd, sizeof(new_cwd)) != NULL)
		new_pwd = ft_strdup(new_cwd);
	else
	{
		if (target[0] == '/')
			new_pwd = ft_strdup(target);
		else
		{
			char *temp = ft_strjoin(old_pwd, "/");
			if (temp != NULL)
			{
				new_pwd = ft_strjoin(temp, target);
				free(temp);
			}
			else
				new_pwd = NULL;
		}
	}
	if (new_pwd == NULL)
	{
		perror("cd: getcwd");
		free(old_pwd);
		return (envp);
	}
	envp = update_env_var(envp, "OLDPWD", old_pwd);
	envp = update_env_var(envp, "PWD", new_pwd);
	free(old_pwd);
	free(new_pwd);
	return (envp);
}

static char	**builtin_pwd(char **args, char **envp)
{
	char	cwd[1024];

	(void)args;
	(void)envp;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("pwd");
		*exit_status() = 1;
		return (envp);
	}
	printf("%s\n", cwd);
	*exit_status() = 0;
	return (envp);
}

static char	**builtin_env(char **args, char **envp)
{
	int	i;

	(void)args;
	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strchr(envp[i], '=') != NULL)
			printf("%s\n", envp[i]);
		i++;
	}
	*exit_status() = 0;
	return (envp);
}

static char	**builtin_exit(char **args, char **envp)
{
	int	status;

	(void)envp;
	printf("exit\n");
	if (args[1] == NULL)
		exit(0);
	if (ft_strcmp(args[1], "-") == 0 || !ft_isdigit(args[1][0]))
		exit(255);
	status = ft_atoi(args[1]);
	exit(status & 0xFF);
	return (envp);
}

char	**exec_builtin(char **args, char **envp)
{
	if (args == NULL || args[0] == NULL)
		return (envp);
	if (ft_strcmp(args[0], "echo") == 0)
	{
		builtin_echo(args);
		return (envp);
	}
	else if (ft_strcmp(args[0], "cd") == 0)
		return (builtin_cd(args, envp));
	else if (ft_strcmp(args[0], "pwd") == 0)
	{
		builtin_pwd(args, envp);
		return (envp);
	}
	else if (ft_strcmp(args[0], "env") == 0)
	{
		builtin_env(args, envp);
		return (envp);
	}
	else if (ft_strcmp(args[0], "exit") == 0)
	{
		builtin_exit(args, envp);
		return (envp);
	}
	return (envp);
}