/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 22:06:09 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/07 00:40:51 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	save_stdio(int saved[2])
{
	saved[0] = dup(STDIN_FILENO);
	if (saved[0] < 0)
	{
		perror("minishell: dup");
		return (-1);
	}
	saved[1] = dup(STDOUT_FILENO);
	if (saved[1] < 0)
	{
		perror("minishell: dup");
		close(saved[0]);
		return (-1);
	}
	return (0);
}

int	restore_stdio(int saved[2])
{
	if (saved[0] >= 0)
	{
		if (dup2(saved[0], STDIN_FILENO) < 0)
		{
			perror("minishell:dup2");
			close(saved[0]);
			return (-1);
		}
		close(saved[0]);
	}
	if (saved[1] >= 0)
	{
		if (dup2(saved[1], STDOUT_FILENO) < 0)
		{
			perror("minishell: dup2");
			close(saved[1]);
			return (-1);
		}
		close(saved[1]);
	}
	return (0);
}

static int	open_redir_fd(t_redir *r)
{
	int	fd;

	fd = -1;
	if (r->type == R_IN)
		fd = open(r->target, O_RDONLY);
	else if (r->type == R_OUT)
		fd = open(r->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (r->type == R_APP)
		fd = open(r->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (r->type == R_HEREDOC)
		fd = open(r->target, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(r->target);
	}
	return (fd);
}

static bool	dup_and_close(int fd, int dest)
{
	if (dup2(fd, dest) < 0)
	{
		perror("minishell: dup2");
		close(fd);
		return (false);
	}
	close(fd);
	return (true);
}

bool	apply_redirections(t_redir *redirs)
{
	t_redir	*cur;
	int		fd;
	int		dest;

	cur = redirs;
	while (cur)
	{
		fd = open_redir_fd(cur);
		if (fd < 0)
			return (false);
		if (cur->type == R_IN || cur->type == R_HEREDOC)
			dest = STDIN_FILENO;
		else
			dest = STDOUT_FILENO;
		if (!dup_and_close(fd, dest))
			return (false);
		cur = cur->next;
	}
	return (true);
}
