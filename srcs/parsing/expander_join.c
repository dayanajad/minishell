/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_join.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:00:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/26 12:00:00 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	should_join(t_tok *cur, t_tok *next)
{
	if (cur->type != TOK_WORD || next->type != TOK_WORD)
		return (false);
	return (cur->join_next);
}

static void	handle_dollar_quote(t_tok *cur, t_tok *next)
{
	if (!cur->was_quoted && ft_strcmp(cur->value, "$") == 0 && next->was_quoted)
	{
		free(cur->value);
		cur->value = ft_strdup("");
	}
}

static void	merge_tokens(t_tok *cur, t_tok *next)
{
	char	*joined;

	handle_dollar_quote(cur, next);
	joined = ft_strjoin(cur->value, next->value);
	free(cur->value);
	cur->value = joined;
	cur->join_next = next->join_next;
	cur->has_wildcard = (cur->has_wildcard || next->has_wildcard);
	cur->was_quoted = (cur->was_quoted || next->was_quoted);
	cur->next = next->next;
	free(next->value);
	free(next);
}

void	join_tokens(t_tok **tokens)
{
	t_tok	*cur;
	t_tok	*next;

	cur = *tokens;
	while (cur && cur->next)
	{
		next = cur->next;
		if (should_join(cur, next))
		{
			merge_tokens(cur, next);
			continue ;
		}
		cur = cur->next;
	}
}
