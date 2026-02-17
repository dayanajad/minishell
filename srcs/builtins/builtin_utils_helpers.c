/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:10:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:05:57 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_builtin_echo(t_cmd *cmd, t_shell *shell)
{
	(void)shell;
	return (builtin_echo(cmd->av));
}
