/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctaboada <ctaboada@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:42:41 by ctaboada          #+#    #+#             */
/*   Updated: 2025/10/14 11:30:46 by ctaboada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_builtin_pwd(void)
{
	char cwd[512];

	if(getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return(0);
	}
	else
	{
		perror("pwd");
		return(1);
	}
}