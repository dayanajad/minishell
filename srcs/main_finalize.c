/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_finalize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:10:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:05:57 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	finalize_shell(t_shell *shell)
{
	if (shell->stdout_filter_pid > 0)
	{
		close(STDOUT_FILENO);
		waitpid(shell->stdout_filter_pid, NULL, 0);
	}
	free_env(shell->env);
}
