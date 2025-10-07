/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctaboada <ctaboada@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:42:44 by ctaboada          #+#    #+#             */
/*   Updated: 2025/10/07 13:31:38 by ctaboada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <limits.h>

int ft_builtin_cd(char **args, t_data *data)
{
	char	oldpwd[MAX_PROMPT_SIZE];
	char	newpwd[MAX_PROMPT_SIZE];
	char	*path;
	char	*oldpwd_str;
	char	*pwd_str;

	// 1️⃣ Obtener la ruta actual antes de cambiar
	if (!getcwd(oldpwd, sizeof(oldpwd)))
	{
		perror("cd: getcwd");
		return (1);
	}

	// 2️⃣ Determinar el destino (por defecto $HOME)
	if (!args[1] || !args[1][0])
	{
		path = getenv("HOME"); // puedes reemplazarlo por tu propia función que busque en data->env
		if (!path)
		{
			fprintf(stderr, "cd: HOME not set\n");
			return (1);
		}
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		// caso especial: cd -
		path = NULL;
		for (int i = 0; data->env[i]; i++)
		{
			if (ft_strncmp(data->env[i], "OLDPWD=", 7) == 0)
			{
				path = data->env[i] + 7;
				break;
			}
		}
		if (!path)
		{
			fprintf(stderr, "cd: OLDPWD not set\n");
			return (1);
		}
		printf("%s\n", path); // mostrar la ruta cuando haces "cd -"
	}
	else
		path = args[1];

	// 3️⃣ Intentar cambiar al nuevo directorio
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}

	// 4️⃣ Obtener la nueva ruta
	if (!getcwd(newpwd, sizeof(newpwd)))
	{
		 if (ft_strcmp(path, "..") == 0)
        ft_strrmdir(oldpwd); // eliminar último segmento de oldpwd
    else
        ft_strjoin_path(oldpwd, path, newpwd); // oldpwd + "/" + path
	}

	// 5️⃣ Actualizar variables de entorno PWD y OLDPWD
	oldpwd_str = ft_strjoin("OLDPWD=", oldpwd);
	if (!oldpwd_str)
		return (1);
	data->env = add_or_update_env(data->env, oldpwd_str);
	free(oldpwd_str);

	pwd_str = ft_strjoin("PWD=", newpwd);
	if (!pwd_str)
		return (1);
	data->env = add_or_update_env(data->env, pwd_str);
	free(pwd_str);

	return (0);
}