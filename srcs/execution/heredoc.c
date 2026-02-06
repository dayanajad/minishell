/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 21:53:47 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/12 22:08:49 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*trim_nl(char *line)
{
	size_t	len;

	if (!line)
		return (NULL);
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	return (line);
}

static int	is_heredoc_delim(char *line, const char *delim)
{
	trim_nl(line);
	return (ft_strcmp(line, delim) == 0);
}

static void	handle_heredoc_eof(const char *delim, t_shell *shell)
{
	if (!shell->heredoc_eof)
		warn_heredoc_eof(delim);
	shell->heredoc_eof = 1;
}

char	*read_heredoc(const char *delimiter, t_shell *shell, bool expand)
{
	int		fd;
	char	*temp_path;
	char	*line;

	fd = open_temp_file(&temp_path);
	if (fd < 0)
		return (NULL);
	while (1)
	{
		line = read_heredoc_raw();
		if (!line)
		{
			handle_heredoc_eof(delimiter, shell);
			break ;
		}
		if (is_heredoc_delim(line, delimiter))
			return (free(line), close(fd), temp_path);
		write_heredoc_line(fd, line, shell, expand);
		free(line);
	}
	close(fd);
	return (temp_path);
}
