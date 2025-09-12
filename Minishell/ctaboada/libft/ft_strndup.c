/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:07:57 by nikotina          #+#    #+#             */
/*   Updated: 2025/09/12 10:08:28 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"
#include <string.h>

char *ft_strndup(const char *s, size_t n)
{
	size_t len = 0;
	char *dup;
	if (s == NULL)
		return NULL;
	while (len < n && s[len])
		len++;
	dup = (char *)malloc(len + 1);
	if (!dup)
		return NULL;
	if (len > 0)
		memcpy(dup, s, len);
	dup[len] = '\0';
	return (dup);
}