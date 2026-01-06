/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 00:44:13 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/07 00:44:16 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(char **av, t_env **env)
{
	(void)av;
	(void)env;
	ft_putstr_fd("unset: not implemented yet\n", STDERR_FILENO);
	return (1);
}
