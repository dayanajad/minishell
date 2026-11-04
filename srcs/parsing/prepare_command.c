/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:00:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:05:57 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expand_heredoc_lines(int fd, int output, t_shell *shell)
{
	char	*line;
	ssize_t	length;

	line = read_line_nobuf(fd);
	while (line)
	{
		line = expand_heredoc_str(line, shell);
		if (!line)
			return (0);
		length = ft_strlen(line);
		if (write(output, line, length) != length)
		{
			free(line);
			return (0);
		}
		free(line);
		line = read_line_nobuf(fd);
	}
	return (1);
}

static int	expand_heredoc_file(t_tok *target, t_shell *shell)
{
	int		fd;
	int		output;
	char	*path;
	int		ok;

	fd = open(target->value, O_RDONLY);
	if (fd < 0)
		return (0);
	output = open_temp_file(&path);
	if (output < 0)
		return (close(fd), 0);
	ok = expand_heredoc_lines(fd, output, shell);
	close(fd);
	close(output);
	unlink(target->value);
	free(target->value);
	target->value = path;
	return (ok);
}

static int	prepare_heredocs(t_tok *tokens, t_shell *shell)
{
	while (tokens)
	{
		if (tokens->type == TOK_HEREDOC && tokens->next
			&& !tokens->next->was_quoted
			&& !expand_heredoc_file(tokens->next, shell))
			return (0);
		tokens = tokens->next;
	}
	return (1);
}

int	prepare_command(t_cmd *cmd, t_shell *shell)
{
	t_tok	*tokens;
	t_tok	*cur;
	int		ok;

	if (!cmd->raw_toks)
		return (1);
	tokens = cmd->raw_toks;
	cmd->raw_toks = NULL;
	if (!prepare_heredocs(tokens, shell))
		return (free_tokens(tokens), 0);
	expand_tokens(&tokens, shell);
	expand_wildcards(&tokens);
	remove_quotes(tokens);
	cur = tokens;
	ok = fill_simple_cmd(&cur, cmd, NULL);
	free_tokens(tokens);
	return (ok);
}
