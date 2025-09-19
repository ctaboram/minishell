/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctaboada <ctaboada@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:42:37 by ctaboada          #+#    #+#             */
/*   Updated: 2025/09/19 10:50:26 by ctaboada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_builting_echo(char **args)
{
	int	i = 1;
	int	newline = 1;

	while (args[i] && !ft_strncmp(args[i], "-n", 2) && args[i][2] == '\0')
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		char *str = args[i];
		size_t len = ft_strlen(str);
		if ((str[0] == '"' && str[len - 1] == '"') || (str[0] == '\'' && str[len - 1] == '\''))
			write(1, str + 1, len - 2);
		else
			printf("%s", str);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}
