/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:00:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/26 12:00:00 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_heredoc_delim(t_tok *target, bool *expand)
{
	*expand = !target->was_quoted;
	if (!*expand)
		return (target->value);
	return (unescape_backslashes(target->value));
}

bool	handle_heredoc_redir(t_tok **cur, t_redir **redirs,
		t_shell *shell, int fd)
{
	t_tok	*target;
	char	*temp_path;
	bool	expand;
	char	*delimiter;

	target = *cur;
	if (!target || target->type != TOK_WORD)
	{
		syntax_err_tok(target);
		return (false);
	}
	delimiter = get_heredoc_delim(target, &expand);
	if (!delimiter)
		return (false);
	temp_path = read_heredoc(delimiter, shell, expand);
	if (expand)
		free(delimiter);
	if (!temp_path)
		return (false);
	append_redir(redirs, new_redir(R_HEREDOC, fd, temp_path));
	free(temp_path);
	*cur = target->next;
	return (true);
}
