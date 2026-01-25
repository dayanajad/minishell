/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 02:15:39 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 02:15:39 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_bg_node(t_ast *ast, t_shell *shell)
{
	pid_t	pid;
	int		outpipe[2];
	bool	capture;

	if (!ast || !shell)
		return (0);
	capture = (!isatty(STDIN_FILENO));
	if (!exec_bg_open_pipe(capture, outpipe))
		return (1);
	pid = fork();
	if (pid < 0)
	{
		if (capture)
			exec_bg_close_pipe(outpipe);
		perror("fork");
		return (1);
	}
	if (pid == 0)
		exec_bg_child(ast, shell, outpipe, capture);
	return (exec_bg_parent(ast, shell, outpipe, pid));
}
