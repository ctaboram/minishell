/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctaboada <ctaboada@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:42:44 by ctaboada          #+#    #+#             */
/*   Updated: 2025/09/16 10:42:45 by ctaboada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_builtin_cd(char **args)
{
	char *target;
	char cwd[1024];
	
	target = args[1];
	if (!target || ft_strcmp(target, "") == 0 || ft_strcmp(target, "~") == 0)
	{
		target = getenv("HOME");
		if (!target)
		{
			ft_printf("cd: HOME not set\n");
			return (1);
		}
	}
	if (chdir(target) != 0)
	{
		perror("cd");
		return (1);
	}
	if (getcwd(cwd, sizeof(cwd)))
		ft_printf("%s\n", cwd);
	return (0);
}