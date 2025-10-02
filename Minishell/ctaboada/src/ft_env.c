/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctaboada <ctaboada@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:42:31 by ctaboada          #+#    #+#             */
/*   Updated: 2025/09/23 13:11:34 by ctaboada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_builtin_env(char **env)
{
	int	i;

	i = 0;
	if(env == NULL)
	{
		perror("env");
		return(1);
	}
	while(env[i] && env)
	{
		printf("%s\n",env[i]);
		i++;
	}
	return(0);
}

char **dup_env(char **env)
{
    int count;
    char **new_env;
    int i;

    if (!env)
        return (NULL);
    count = 0;
    while (env[count])
        count++;
    new_env = (char **)malloc((count + 1) * sizeof(char *));
    if (!new_env)
        return (NULL);
    i = 0;
    while (i < count)
    {
        new_env[i] = ft_strdup(env[i]);
        if (!new_env[i])
        {
            while (--i >= 0)
                free(new_env[i]);
            free(new_env);
            return (NULL);
        }
        i++;
    }
    new_env[count] = NULL;
    return (new_env);
}
