/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bg_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:20:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:05:57 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_bg_open_pipe(bool capture, int outpipe[2])
{
	if (!capture)
		return (1);
	if (pipe(outpipe) < 0)
	{
		perror("minishell: pipe");
		return (0);
	}
	return (1);
}

void	exec_bg_close_pipe(int outpipe[2])
{
	close(outpipe[0]);
	close(outpipe[1]);
}

void	exec_bg_flush_output(int fd)
{
	char	buf[4096];
	ssize_t	n;

	n = 1;
	while (n > 0)
	{
		n = read(fd, buf, sizeof(buf));
		if (n > 0)
			write(STDOUT_FILENO, buf, (size_t)n);
	}
}
