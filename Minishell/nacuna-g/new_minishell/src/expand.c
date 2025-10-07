/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 11:50:45 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/07 13:14:36 by nacuna-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*expand_word(char *word)
{
	char	*result;
	int		i;

	result = malloc(sizeof(char) * 1);
	i = 0;
	while (word[i])
	{
		if (word[i] == '$')
		{
			i++;
			
		}
	}
}
