/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:55:26 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/16 13:04:18 by nacuna-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char *ft_strjoin_free(char *s1, char *s2)
{
	char *new;

	new = ft_strjoin(s1, s2);
	free(s1);
	return (new);
}
