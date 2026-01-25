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

static int	restore_one_fd(int saved, int target)
{
	if (saved >= 0)
	{
		if (dup2(saved, target) < 0)
		{
			perror("minishell: dup2");
			close(saved);
			return (-1);
		}
		close(saved);
	}
	return (0);
}

int	save_stdio(int saved[3])
{
	saved[0] = dup(STDIN_FILENO);
	if (saved[0] < 0)
		return (perror("minishell: dup"), -1);
	saved[1] = dup(STDOUT_FILENO);
	if (saved[1] < 0)
	{
		perror("minishell: dup");
		close(saved[0]);
		return (-1);
	}
	saved[2] = dup(STDERR_FILENO);
	if (saved[2] < 0)
	{
		perror("minishell: dup");
		close(saved[0]);
		close(saved[1]);
		return (-1);
	}
	return (0);
}

int	restore_stdio(int saved[3])
{
	if (restore_one_fd(saved[0], STDIN_FILENO) < 0)
		return (-1);
	if (restore_one_fd(saved[1], STDOUT_FILENO) < 0)
		return (-1);
	if (restore_one_fd(saved[2], STDERR_FILENO) < 0)
		return (-1);
	return (0);
}

static int	open_redir_fd(t_redir *r, t_shell *shell)
{
	int		fd;
	char	**matches;
	char	*target;

	if (!resolve_redir_target(r, &target, &matches))
		return (-1);
	fd = -1;
	if (r->type == R_IN || r->type == R_HEREDOC)
		fd = open(target, O_RDONLY);
	else if (r->type == R_OUT)
		fd = open(target, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else if (r->type == R_APP)
		fd = open(target, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (ft_strchr(r->target, '*') && r->type != R_HEREDOC)
		free_str_arr(matches);
	if (fd < 0 && !(shell && shell->heredoc_eof && !isatty(STDIN_FILENO)))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(target);
	}
	return (fd);
}

bool	apply_redirections(t_redir *redirs, t_shell *shell)
{
	t_redir	*cur;
	int		fd;

	cur = redirs;
	while (cur)
	{
		fd = open_redir_fd(cur, shell);
		if (fd < 0)
			return (false);
		if (!dup_and_close(fd, cur->fd))
			return (false);
		cur = cur->next;
	}
	return (true);
}
