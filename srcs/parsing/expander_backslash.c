/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_backslash.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:00:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/26 12:00:00 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*make_backslashes(int count)
{
	char	*out;
	int		j;

	if (count <= 0)
		return (ft_strdup(""));
	out = ft_calloc(count + 1, sizeof(char));
	if (!out)
		return (NULL);
	j = 0;
	while (j < count)
		out[j++] = '\\';
	return (out);
}

static int	is_dq_escapable(char next)
{
	if (next == '\\' || next == '"' || next == '$' || next == '\n')
		return (1);
	return (0);
}

static char	*handle_odd_backslash(int count, char next)
{
	char	*prefix;
	char	*ch;
	char	*joined;
	int		literal;

	literal = count / 2;
	prefix = make_backslashes(literal);
	if (next == '\n')
		return (prefix);
	ch = process_char(next);
	joined = append_val(prefix, ch);
	free(ch);
	return (joined);
}

static char	*handle_backslash_end(char *s, int *i, int count, bool dq)
{
	char	next;
	int		literal;

	next = s[*i + count];
	if (!next)
	{
		*i += count;
		return (make_backslashes(count));
	}
	if (dq && !is_dq_escapable(next))
	{
		*i += count;
		return (make_backslashes(count));
	}
	literal = count / 2;
	if (count % 2 == 0)
	{
		*i += count;
		return (make_backslashes(literal));
	}
	*i += count + 1;
	return (handle_odd_backslash(count, next));
}

char	*process_backslashes(char *s, int *i, bool sq, bool dq)
{
	int		count;

	if (sq)
		return (process_char(s[(*i)++]));
	count = 0;
	while (s[*i + count] == '\\')
		count++;
	return (handle_backslash_end(s, i, count, dq));
}
