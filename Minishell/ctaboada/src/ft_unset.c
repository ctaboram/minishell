/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctaboada <ctaboada@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 12:30:20 by ctaboada          #+#    #+#             */
/*   Updated: 2025/09/23 13:02:20 by ctaboada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	count_env_vars(char **env)
{
    int	size;

    if (!env)
        return (0);
    size = 0;
    while (env[size])
        size++;
    return (size);
}

static int	get_var_name_length(char *str)
{
    int	i;

    i = 0;
    while (str[i] && str[i] != '=')
        i++;
    return (i);
}

static char	**create_new_env(char **env, char *var_to_remove)
{
    int		i;
    int		j;
    int		var_len;
    int		env_var_len;
    char	**new_env;

    var_len = ft_strlen(var_to_remove);
    new_env = malloc(sizeof(char *) * (count_env_vars(env) + 1));
    if (!new_env)
        return (NULL);
    i = 0;
    j = 0;
    while (env[i])
    {
        env_var_len = get_var_name_length(env[i]);
        if (ft_strncmp(env[i], var_to_remove, var_len) != 0 || 
            var_len != env_var_len)
        {
            new_env[j] = ft_strdup(env[i]);
            if (!new_env[j])
            {
                ft_free_env(new_env);
                return (NULL);
            }
            j++;
        }
        i++;
    }
    new_env[j] = NULL;
    return (new_env);
}

char	**ft_builtin_unset(char **args, char **env)
{
    int		i;
    char	**new_env;
    char	**temp;

    if (!args || !env || !args[1])
        return (env);
    i = 1;
    while (args[i])
    {
        if (!is_valid_identifier(args[i]))
        {
            printf("unset: `%s': not a valid identifier\n", args[i]);
            i++;
            continue;
        }
        new_env = create_new_env(env, args[i]);
        if (!new_env)
            return (env);
      	if(new_env != env)
	  	{
			temp = env;
			env = new_env;
			ft_free_env(temp);
		}
        i++;
    }
    return (env);
}