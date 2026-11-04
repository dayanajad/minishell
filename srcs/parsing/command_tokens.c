/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:00:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:05:57 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_tok	*copy_raw_token(t_tok *tok)
{
	t_tok	*copy;

	copy = malloc(sizeof(*copy));
	if (!copy)
		return (NULL);
	*copy = *tok;
	copy->value = NULL;
	if (tok->value)
		copy->value = ft_strdup(tok->value);
	copy->next = NULL;
	if (tok->value && !copy->value)
	{
		free(copy);
		return (NULL);
	}
	return (copy);
}

int	copy_command_tokens(t_tok **cur, t_tok **tokens)
{
	t_tok	*copy;
	t_tok	**tail;

	tail = tokens;
	while (*cur && !is_cmd_end(*cur))
	{
		copy = copy_raw_token(*cur);
		if (!copy)
			return (0);
		*tail = copy;
		tail = &copy->next;
		*cur = (*cur)->next;
	}
	return (1);
}

static int	join_heredoc_delimiter(t_tok *target)
{
	t_tok	*next;
	char	*joined;

	while (target->join_next && target->next
		&& target->next->type == TOK_WORD)
	{
		next = target->next;
		joined = ft_strjoin(target->value, next->value);
		if (!joined)
			return (0);
		free(target->value);
		target->value = joined;
		target->was_quoted |= next->was_quoted;
		target->join_next = next->join_next;
		target->next = next->next;
		free(next->value);
		free(next);
	}
	return (1);
}

static int	collect_heredoc(t_tok *target, t_shell *shell)
{
	char	*path;

	if (!target || target->type != TOK_WORD
		|| !join_heredoc_delimiter(target))
		return (0);
	if (!normalize_heredoc_delimiter(target))
		return (0);
	path = read_heredoc(target->value, shell, false);
	if (!path)
		return (0);
	free(target->value);
	target->value = path;
	return (1);
}

int	collect_command_heredocs(t_tok *tokens, t_shell *shell)
{
	while (tokens)
	{
		if (tokens->type == TOK_HEREDOC
			&& !collect_heredoc(tokens->next, shell))
			return (0);
		tokens = tokens->next;
	}
	return (1);
}
