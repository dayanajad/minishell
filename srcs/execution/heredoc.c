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

static void	write_heredoc_line(int fd, char *line, t_shell *shell,
		bool expand)
{
	char	*expanded;
	char	*dup;

	if (expand)
	{
		dup = ft_strdup(line);
		if (!dup)
			return ;
		expanded = expand_str(dup, shell);
	}
	else
		expanded = ft_strdup(line);
	if (!expanded)
		return ;
	write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
	free(expanded);
}

static void	read_heredoc_loop(int fd, const char *delimiter, t_shell *shell,
		bool expand)
{
	char	*line;
	char	*raw;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			raw = readline("> ");
		else
			raw = read_line_nobuf(STDIN_FILENO);
		line = raw;
		if (!line)
		{
			if (!shell->heredoc_eof)
				warn_heredoc_eof(delimiter);
			shell->heredoc_eof = 1;
			break ;
		}
		trim_nl(line);
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write_heredoc_line(fd, line, shell, expand);
		free(line);
	}
}

char	*read_heredoc(const char *delimiter, t_shell *shell, bool expand)
{
	int		fd;
	char	*temp_path;

	fd = open_temp_file(&temp_path);
	if (fd < 0)
		return (NULL);
	read_heredoc_loop(fd, delimiter, shell, expand);
	close(fd);
	return (temp_path);
}
