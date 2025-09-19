/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctaboada <ctaboada@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 12:56:07 by ctaboada          #+#    #+#             */
/*   Updated: 2025/09/19 14:25:53 by ctaboada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Validar si el identificador es correcto para export */
int is_valid_identifier(char *arg)
{
    int i;

    if (!arg || !arg[0] || (!ft_isalpha(arg[0]) && arg[0] != '_'))
        return (0);

    i = 1;
    while (arg[i] && arg[i] != '=')
    {
        if (!ft_isalnum(arg[i]) && arg[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

/* Crear un nuevo array de entorno con la variable añadida */
static char **add_new_env(char **env, char *new_var)
{
    int count = 0;
    char **new_env;
    int i = 0;

    while (env && env[count])
        count++;

    new_env = malloc((count + 2) * sizeof(char *));
    if (!new_env)
        return (NULL);

    while (i < count)
    {
        new_env[i] = ft_strdup(env[i]);
        i++;
    }

    new_env[i] = ft_strdup(new_var);
    new_env[i + 1] = NULL;

    return new_env;
}

/* Añadir o actualizar variable en el entorno */
char **add_or_update_env(char **env, char *var)
{
    int i = 0;
    char *key;
    char *equal_sign;
    char **new_env;

    equal_sign = ft_strchr(var, '=');
    if (!equal_sign)
        return (env); // no es variable asignable

    key = strndup(var, equal_sign - var);
    if (!key)
        return (env); // fallo en malloc

    while (env && env[i])
    {
        if (ft_strncmp(env[i], key, ft_strlen(key)) == 0
            && (env[i][ft_strlen(key)] == '=' || env[i][ft_strlen(key)] == '\0'))
        {
            free(env[i]);
            env[i] = ft_strdup(var);
            free(key);
            return (env);
        }
        i++;
    }

    new_env = add_new_env(env, var);
    if (!new_env)
    {
        free(key);
        return (env);
    }

    ft_free_env(env); // liberar array viejo

    free(key);
    return new_env;
}

/* Mostrar el entorno ordenado para export sin argumento */
void ft_export_print(char **env)
{
    int i = 0;

    while (env && env[i])
    {
        printf("declare -x %s\n", env[i]);
        i++;
    }
}

