/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_tok_split_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:00:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/26 12:00:00 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*whitespace_to_space(const char *s)
{
	char	*tmp;
	int		i;

	if (!s)
		return (NULL);
	tmp = ft_strdup(s);
	if (!tmp)
		return (NULL);
	i = 0;
	while (tmp[i])
	{
		if (tmp[i] == '\t' || tmp[i] == '\n')
			tmp[i] = ' ';
		i++;
	}
	return (tmp);
}

t_tok	*handle_empty_split(char **parts, t_tok **tokens, t_tok *prev,
		t_tok *next_tok)
{
	free_str_arr(parts);
	if (prev)
		prev->next = next_tok;
	else
		*tokens = next_tok;
	return (prev);
}

static bool	word_has_wc(const char *s)
{
	if (!s)
		return (false);
	return (ft_strchr(s, '*') != NULL);
}

t_tok	*make_split_tok(char *word)
{
	t_tok	*node;
	char	*dup;

	dup = ft_strdup(word);
	if (!dup)
		return (NULL);
	node = new_tok(TOK_WORD, dup, 0);
	if (!node)
	{
		free(dup);
		return (NULL);
	}
	node->has_wildcard = word_has_wc(node->value);
	node->was_quoted = false;
	node->join_next = false;
	return (node);
}
