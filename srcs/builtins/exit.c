/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 00:41:17 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/07 00:41:19 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_exit(char **av, t_shell *shell)
{
	(void)av;
	ft_putstr_fd("exit\n", STDERR_FILENO);
	shell->should_exit = 1;
	return (0);
}
