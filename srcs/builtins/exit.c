/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 05:28:17 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/12/11 05:28:18 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_numeric(const char *s)
{
	size_t	i;

	if (!s || !*s)
		return (false);
	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!s[i])
		return (false);
	while (s[i])
	{
		if (!ft_isdigit((unsigned char)s[i]))
			return (false);
		i++;
	}
	return (true);
}

int	builtin_exit(char **av, t_shell *shell)
{
	int	code;

	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (!av[1])
	{
		shell->should_exit = 1;
		return (0);
	}
	if (!is_numeric(av[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(av[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		shell->should_exit = 1;
		return (2);
	}
	if (av[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n",
			STDERR_FILENO);
		return (1);
	}
	code = ft_atoi(av[1]);
	shell->should_exit = 1;
	return ((int)(code & 0xff));
}
