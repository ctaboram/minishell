/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctaboada <ctaboada@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:55:49 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/23 13:16:49 by ctaboada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	char **g_env;
	
	g_env = dup_env(env);
	printf(HEADER);
	while (ft_prompt(g_env))
		;
	return (0);
}
