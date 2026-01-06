/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 00:43:53 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/07 00:43:54 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_export(char **av, t_env **env)
{
	(void)av;
	(void)env;
	ft_putstr_fd("export: not implemented yet\n", STDERR_FILENO);
	return (1);
}
