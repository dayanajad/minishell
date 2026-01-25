/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 23:35:33 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/07 00:06:32 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_echo_n_flag(const char *s)
{
	int	i;

	if (!s || s[0] != '-' || s[1] != 'n')
		return (false);
	i = 2;
	while (s[i])
	{
		if (s[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

int	builtin_echo(char **av)
{
	int	i;
	int	nl;

	nl = 1;
	i = 1;
	while (av[i] && is_echo_n_flag(av[i]))
	{
		nl = 0;
		i++;
	}
	if (write_echo_args(av, i) != 0)
		return (1);
	if (nl)
	{
		if (echo_write_ch('\n') < 0)
			return (1);
	}
	return (0);
}
