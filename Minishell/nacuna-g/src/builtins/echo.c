/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctaboada <ctaboada@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:42:37 by ctaboada          #+#    #+#             */
/*   Updated: 2025/10/14 11:30:39 by ctaboada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_builtin_echo(char **args)
{
	int		i;
	int		newline;
	char	*str;
	size_t	len;

	i = 1;
	newline = 1;
	while (args[i] && !ft_strncmp(args[i], "-n", 2) && args[i][2] == '\0')
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		str = args[i];
		len = ft_strlen(str);
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
