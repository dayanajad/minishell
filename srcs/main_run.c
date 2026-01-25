/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_run.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:10:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:05:57 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_shell(t_shell *shell)
{
	char	*line;
	bool	interactive;

	interactive = isatty(STDIN_FILENO);
	while (!shell->should_exit)
	{
		set_signal(0);
		line = read_input_line(shell);
		if (!line)
		{
			if (interactive)
				printf("exit\n");
			break ;
		}
		if (get_signal() == SIGINT)
			shell->last_status = 130;
		process_line(line, shell);
		free(line);
	}
	return (shell->last_status);
}
