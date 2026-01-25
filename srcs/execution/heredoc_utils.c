/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:10:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:05:57 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_temp_file(void)
{
	static int	counter = 0;
	char		*num_str;
	char		*temp_path;
	char		*tmp;

	num_str = ft_itoa(counter++);
	if (!num_str)
		return (NULL);
	tmp = ft_strjoin("/tmp/.minishell_heredoc_", num_str);
	free(num_str);
	if (!tmp)
		return (NULL);
	temp_path = ft_strjoin(tmp, "_XXXXXX");
	free(tmp);
	return (temp_path);
}

int	open_temp_file(char **path)
{
	int		fd;
	char	*temp_template;

	temp_template = create_temp_file();
	if (!temp_template)
		return (-1);
	fd = mkstemp(temp_template);
	if (fd < 0)
	{
		free(temp_template);
		perror("minishell: mkstemp");
		return (-1);
	}
	*path = temp_template;
	return (fd);
}

void	warn_heredoc_eof(const char *delimiter)
{
	ft_putstr_fd("minishell: line 1: warning: ", 2);
	ft_putstr_fd("here-document at line 1 ", 2);
	ft_putstr_fd("delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(delimiter, 2);
	ft_putendl_fd("')", 2);
}

void	write_heredoc_line(int fd, char *line, t_shell *shell, bool expand)
{
	char	*expanded;
	char	*dup;

	if (expand)
	{
		dup = ft_strdup(line);
		if (!dup)
			return ;
		expanded = expand_heredoc_str(dup, shell);
	}
	else
		expanded = ft_strdup(line);
	if (!expanded)
		return ;
	write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
	free(expanded);
}

char	*read_heredoc_raw(void)
{
	if (isatty(STDIN_FILENO))
		return (readline("> "));
	return (read_line_nobuf(STDIN_FILENO));
}
