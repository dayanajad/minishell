/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_tok_split.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:00:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/26 12:00:00 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_tok	*build_split_list(char **parts, t_tok **tokens, t_tok *prev)
{
	t_tok	*last;
	t_tok	*node;
	int		i;

	last = prev;
	i = 0;
	while (parts[i])
	{
		node = make_split_tok(parts[i]);
		if (!node)
			break ;
		if (last)
			last->next = node;
		else
			*tokens = node;
		last = node;
		i++;
	}
	return (last);
}

static bool	prepare_split_context(t_tok *cur, t_tok *prev)
{
	bool	end_ws;

	end_ws = false;
	if (ft_strlen(cur->value) > 0)
		end_ws = (cur->value[ft_strlen(cur->value) - 1] == ' ');
	if (cur->value[0] == ' ' && prev)
		prev->join_next = false;
	return (end_ws);
}

static t_tok	*finalize_split(char **p, t_tok **t, t_tok *pr, t_tok *n)
{
	t_tok	*last;

	if (!p || !p[0])
		return (handle_empty_split(p, t, pr, n));
	last = build_split_list(p, t, pr);
	last->next = n;
	free_str_arr(p);
	return (last);
}

t_tok	*perform_split_tok(t_tok **tokens, t_tok *prev, t_tok *cur)
{
	char	**parts;
	char	*tmp;
	t_tok	*last;
	bool	end_ws;
	bool	orig_join;

	orig_join = cur->join_next;
	end_ws = prepare_split_context(cur, prev);
	tmp = whitespace_to_space(cur->value);
	if (!tmp)
		return (cur);
	parts = ft_split(tmp, ' ');
	free(tmp);
	last = finalize_split(parts, tokens, prev, cur->next);
	free(cur->value);
	free(cur);
	if (!last)
		return (NULL);
	if (end_ws)
		last->join_next = false;
	else
		last->join_next = orig_join;
	return (last);
}
