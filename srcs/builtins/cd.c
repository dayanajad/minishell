/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 00:43:42 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/07 00:43:44 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_cd(char **av, t_shell *shell)
{
	(void)av;
	(void)shell;
	ft_putstr_fd("cd: not implemented yet\n", STDERR_FILENO);
	return (1);
}
