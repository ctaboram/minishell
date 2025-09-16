/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctaboada <ctaboada@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:42:31 by ctaboada          #+#    #+#             */
/*   Updated: 2025/09/16 12:25:55 by ctaboada         ###   ########.fr       */
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
	while(env[i] != NULL)
	{
		printf("%s\n",env[i]);
		i++;
	}
	return(0);
}

char **dup_env(char **env)
{
	int count;
	int i;
	char **copy;

	count = 0;
	i = 0;
	while(env && env[count])
		count++;
	copy = malloc((count + 1)*sizeof(char * ));
	if(!copy)
		return(NULL);
	while(i < count)
	{
		copy[i] = ft_strdup(env[i]);
		i++;
	}
	copy[count] = NULL;
	return(copy);
}