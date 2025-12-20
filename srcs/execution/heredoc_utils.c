/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 07:22:37 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/12/11 08:26:16 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*heredoc_make_filename(int index)
{
	char	*idx_str;
	char	*name;

	idx_str = ft_itoa(index);
	if (!idx_str)
		return (NULL);
	name = ft_strjoin("/tmp/.minishell_heredoc_", idx_str);
	free(idx_str);
	return (name);
}

static int	heredoc_write_loop(int fd, const char *delim, t_shell *shell)
{
	char	*line;

	(void)shell;
	while (1)
	{
		if (g_signal == SIGINT)
			return (0);
		line = readline("> ");
		if (!line)
			return (1);
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
	}
	return (1);
}

static void	heredoc_add_node(t_shell *shell, const char *path)
{
	t_hdoc	*node;

	node = malloc(sizeof(*node));
	if (!node)
		return ;
	node->path = ft_strdup(path);
	if (!node->path)
	{
		free(node);
		return ;
	}
	node->next = shell->heredocs;
	shell->heredocs = node;
}

int	heredoc_create_file(t_redir *redir, int index, t_shell *shell)
{
	char	*name;
	int		fd;
	int		ok;

	name = heredoc_make_filename(index);
	if (!name)
		return (0);
	fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		perror("minishell: heredoc");
		return (free(name), 0);
	}
	ok = heredoc_write_loop(fd, redir->target, shell);
	close(fd);
	if (!ok)
	{
		unlink(name);
		return (free(name), 0);
	}
	free(redir->target);
	redir->target = name;
	heredoc_add_node(shell, name);
	return (1);
}

void	cleanup_heredoc_files(t_shell *shell)
{
	t_hdoc	*cur;
	t_hdoc	*next;

	cur = shell->heredocs;
	while (cur)
	{
		next = cur->next;
		if (cur->path)
		{
			unlink(cur->path);
			free(cur->path);
		}
		free(cur);
		cur = next;
	}
	shell->heredocs = NULL;
}
