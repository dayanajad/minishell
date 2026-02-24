/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:10:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:05:57 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sanitize_line(char *line)
{
	int	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
}

static char	*get_first_line(bool interactive)
{
	char	*line;

	if (interactive)
	{
		set_readline_active(1);
		line = readline("minishell$ ");
		set_readline_active(0);
		return (line);
	}
	return (read_line_nobuf(STDIN_FILENO));
}

static char	*read_next_piece(bool interactive)
{
	char	*line;

	if (interactive)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = read_line_nobuf(STDIN_FILENO);
		if (line)
			sanitize_line(line);
		return (line);
	}
	return (read_line_nobuf(STDIN_FILENO));
}

static char	*append_next(char *line, bool interactive, char *quote)
{
	char	*next;

	next = read_next_piece(interactive);
	if (get_signal() == SIGINT)
	{
		free(line);
		free(next);
		return (ft_strdup(""));
	}
	if (!next)
		return (handle_multiline_eof(line, *quote));
	line = join_piece(line, next, interactive);
	if (line)
		*quote = get_open_quote(line);
	return (line);
}

char	*read_input_line(t_shell *shell)
{
	char	*line;
	char	quote;
	bool	interactive;

	(void)shell;
	interactive = isatty(STDIN_FILENO);
	line = get_first_line(interactive);
	quote = 0;
	if (line)
		quote = get_open_quote(line);
	while (line && quote)
	{
		line = append_next(line, interactive, &quote);
		if (!line || !quote)
			break ;
	}
	return (line);
}
