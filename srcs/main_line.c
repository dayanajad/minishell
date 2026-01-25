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

static char	get_open_quote(char *line)
{
	char	quote;

	quote = 0;
	scan_to_comment_or_eof(line, &quote);
	return (quote);
}

static char	*join_piece(char *acc, char *piece, bool insert_newline)
{
	char	*tmp;

	if (!acc)
		return (piece);
	if (insert_newline)
	{
		tmp = ft_strjoin(acc, "\n");
		free(acc);
		if (!tmp)
		{
			free(piece);
			return (NULL);
		}
		acc = tmp;
	}
	tmp = ft_strjoin(acc, piece);
	free(acc);
	free(piece);
	return (tmp);
}

int	is_blank_line(const char *line)
{
	int	i;

	i = 0;
	while (line[i] && ft_strchr(" \t\r\n", line[i]))
		i++;
	return (line[i] == '\0');
}

void	sanitize_line(char *line)
{
	int	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
}

char	*read_input_line(t_shell *shell)
{
	char	*line;
	char	quote;
	char	*next;
	bool	interactive;

	interactive = isatty(STDIN_FILENO);
	if (interactive)
	{
		if (shell && shell->noninteractive_prompt_newline)
		{
			write(STDOUT_FILENO, "\n", 1);
			shell->noninteractive_prompt_newline = 0;
		}
		line = readline("minishell$ ");
	}
	else
	{
		if (shell)
			shell->noninteractive_prompt_newline = 0;
		line = read_line_nobuf(STDIN_FILENO);
	}
	quote = 0;
	if (line)
		quote = get_open_quote(line);
	while (line && quote)
	{
		if (interactive)
			next = readline("> ");
		else
			next = read_line_nobuf(STDIN_FILENO);
		if (!next)
		{
			ft_putstr_fd("minishell: unexpected EOF while looking ", 2);
			ft_putstr_fd("for matching `", 2);
			ft_putchar_fd(quote, 2);
			ft_putendl_fd("'", 2);
			free(line);
			return (NULL);
		}
		line = join_piece(line, next, interactive);
		if (!line)
			return (NULL);
		quote = get_open_quote(line);
	}
	return (line);
}
