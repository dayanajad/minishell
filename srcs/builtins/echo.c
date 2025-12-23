/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 23:35:33 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/12/23 23:38:43 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(char **av)
{
	int	i;
	int	newline;

	newline = 1;
	i = 1;
	if (av[i] && ft_strcmp(av[i], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (av[i])
	{
		ft_putstr_fd(av[i], STDOUT_FILENO);
		if (av[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
