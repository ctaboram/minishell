/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctaboada <ctaboada@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:01:25 by ctaboada          #+#    #+#             */
/*   Updated: 2025/09/16 13:07:35 by ctaboada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_builtin_export(char **arg,char **env)
{
	int		i;
	char	*equal;
	int		key_len;
	i = 0;
	if(!arg[1])
	{
		while(env[i])
		{
			equal = ft_strchr(env[i],'=');
			if(equal)
			{
				key_len = equal - env[i];
				ft_printf("declare -x %.*s=\"%s\"\n",key_len,env[i],equal + 1);
			}
			else
			{
				ft_printf("declare -x %s\n",env[i]);
			}
			i++;
		}
		return 0;
	}
	return 1;
}
