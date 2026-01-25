/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 15:45:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/21 15:45:00 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_redir_prev(t_tok *prev)
{
	if (!prev)
		return (false);
	return (prev->type == TOK_IN || prev->type == TOK_OUT
		|| prev->type == TOK_APP || prev->type == TOK_HEREDOC);
}

static void	expand_word_tok(t_tok **cur)
{
	char	**matches;

	matches = get_matches((*cur)->value);
	if (matches && matches[0])
	{
		insert_matches(cur, matches);
		free_str_arr(matches);
	}
}

void	expand_wildcards(t_tok **tokens)
{
	t_tok	*cur;
	t_tok	*prev;

	cur = *tokens;
	prev = NULL;
	while (cur)
	{
		if (cur->type == TOK_WORD && cur->has_wildcard)
		{
			if (!is_redir_prev(prev))
				expand_word_tok(&cur);
		}
		prev = cur;
		cur = cur->next;
	}
}
