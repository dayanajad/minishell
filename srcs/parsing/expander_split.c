/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:00:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/26 12:00:00 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	has_ifs_space(const char *s)
{
	int	i;

	if (!s)
		return (false);
	i = 0;
	while (s[i])
	{
		if (s[i] == ' ' || s[i] == '\t' || s[i] == '\n')
			return (true);
		i++;
	}
	return (false);
}

static void	remove_empty_tok(t_tok **tokens, t_tok *prev, t_tok *cur)
{
	t_tok	*next;

	next = cur->next;
	if (prev && !cur->join_next)
		prev->join_next = false;
	if (prev)
		prev->next = next;
	else
		*tokens = next;
	free(cur->value);
	free(cur);
}

static int	handle_word_split(t_tok **tokens, t_tok **prev, t_tok **cur)
{
	t_tok	*last;
	t_tok	*next;

	next = (*cur)->next;
	if (*prev && (*prev)->type == TOK_HEREDOC)
		return (0);
	if ((*cur)->value && (*cur)->value[0] == '\0' && !(*cur)->was_quoted)
	{
		remove_empty_tok(tokens, *prev, *cur);
		*cur = next;
		return (1);
	}
	if (!(*cur)->was_quoted && has_ifs_space((*cur)->value))
	{
		last = perform_split_tok(tokens, *prev, *cur);
		if (last)
		{
			*prev = last;
			*cur = last->next;
		}
		else
			*cur = *tokens;
		return (1);
	}
	return (0);
}

void	split_and_clean_toks(t_tok **tokens, t_shell *shell)
{
	t_tok	*cur;
	t_tok	*prev;
	t_tok	*next;

	(void)shell;
	cur = *tokens;
	prev = NULL;
	while (cur)
	{
		next = cur->next;
		if (cur->type == TOK_WORD)
		{
			if (handle_word_split(tokens, &prev, &cur))
				continue ;
		}
		prev = cur;
		cur = next;
	}
}
