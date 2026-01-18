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

static bool	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (false);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
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

static int	calc_exit_code(char *arg)
{
	int	exit_code;

	exit_code = ft_atoi(arg) % 256;
	if (exit_code < 0)
		exit_code += 256;
	return (exit_code);
}

int	builtin_exit(char **av, t_shell *shell)
{
	int	exit_code;

	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (!av[1])
	{
		shell->should_exit = 1;
		return (shell->last_status);
	}
	if (av[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	if (!is_numeric(av[1]))
		return (handle_non_numeric(av[1], shell));
	exit_code = calc_exit_code(av[1]);
	shell->should_exit = 1;
	return (exit_code);
}
