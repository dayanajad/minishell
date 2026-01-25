/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_filter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:10:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:05:57 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	filter_child_loop(int read_fd, int real_out)
{
	char	buf[4096];
	ssize_t	n;
	ssize_t	i;

	n = 1;
	while (n > 0)
	{
		n = read(read_fd, buf, sizeof(buf));
		if (n > 0)
		{
			i = 0;
			while (i < n)
			{
				if (buf[i] != '\0')
					write(real_out, &buf[i], 1);
				i++;
			}
		}
	}
}

static int	setup_filter_pipe(int p[2], int *real_out)
{
	if (pipe(p) < 0)
		return (0);
	*real_out = dup(STDOUT_FILENO);
	if (*real_out < 0)
	{
		close(p[0]);
		close(p[1]);
		return (0);
	}
	return (1);
}

static void	setup_filter_child(int p[2], int real_out)
{
	close(p[1]);
	filter_child_loop(p[0], real_out);
	close(p[0]);
	close(real_out);
	exit(0);
}

static void	setup_filter_parent(t_shell *shell, int p[2], int real_out,
		pid_t pid)
{
	shell->stdout_filter_pid = pid;
	close(p[0]);
	dup2(p[1], STDOUT_FILENO);
	close(p[1]);
	close(real_out);
}

void	setup_stdout_nul_filter(t_shell *shell)
{
	int		p[2];
	int		real_out;
	pid_t	pid;

	if (!shell || isatty(STDOUT_FILENO))
		return ;
	if (!setup_filter_pipe(p, &real_out))
		return ;
	pid = fork();
	if (pid < 0)
	{
		close(p[0]);
		close(p[1]);
		close(real_out);
		return ;
	}
	if (pid == 0)
		setup_filter_child(p, real_out);
	setup_filter_parent(shell, p, real_out, pid);
}
