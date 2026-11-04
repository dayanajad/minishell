/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 14:00:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/11/02 14:00:00 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	wait_heredoc_child(pid_t pid, t_shell *shell)
{
	int	status;
	int	ret;

	signal(SIGINT, SIG_IGN);
	ret = waitpid(pid, &status, 0);
	while (ret == -1 && errno == EINTR)
		ret = waitpid(pid, &status, 0);
	setup_signals_interactive();
	if (ret == -1)
		return (0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		set_signal(SIGINT);
		shell->last_status = 130;
		return (0);
	}
	return (1);
}

int	heredoc_in_child(int fd, const char *delimiter, t_shell *shell,
		bool expand)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		return (0);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		heredoc_loop(fd, delimiter, shell, expand);
		close(fd);
		exit(0);
	}
	return (wait_heredoc_child(pid, shell));
}
