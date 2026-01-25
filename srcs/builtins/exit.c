/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 19:22:48 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/11 19:38:00 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>

static bool	will_overflow(unsigned long long val, int sign, int digit)
{
	unsigned long long	limit;

	if (sign == 1)
		limit = (unsigned long long)LLONG_MAX;
	else
		limit = (unsigned long long)LLONG_MAX + 1;
	if (val > limit / 10)
		return (true);
	if (val == limit / 10 && (unsigned long long)digit > limit % 10)
		return (true);
	return (false);
}

static int	skip_space_sign(const char *str, int *sign)
{
	int	i;

	i = 0;
	*sign = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			*sign = -1;
		i++;
	}
	return (i);
}

static bool	atoll_check(const char *str, long long *res)
{
	unsigned long long	val;
	int					sign;
	int					i;

	val = 0;
	i = skip_space_sign(str, &sign);
	if (!str[i])
		return (false);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		if (will_overflow(val, sign, str[i] - '0'))
			return (false);
		val = val * 10 + (str[i] - '0');
		i++;
	}
	*res = (long long)(val * sign);
	return (true);
}

static int	handle_non_numeric(char *arg, t_shell *shell)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	shell->should_exit = 1;
	return (2);
}

int	builtin_exit(char **av, t_shell *shell)
{
	long long	exit_val;

	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", STDERR_FILENO);
	if (!av[1])
	{
		shell->should_exit = 1;
		return (shell->last_status);
	}
	if (!atoll_check(av[1], &exit_val))
		return (handle_non_numeric(av[1], shell));
	if (av[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	shell->should_exit = 1;
	return ((int)(exit_val % 256 + 256) % 256);
}
