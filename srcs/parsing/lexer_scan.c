/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_scan.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:10:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 18:44:40 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	scan_to_comment_or_eof(char *s, char *quote)
{
	int	len;

	len = 0;
	while (s[len])
	{
		if (!*quote && s[len] == '#'
			&& (len == 0 || ft_strchr(" \t", s[len - 1])))
			break ;
		if (!*quote && (s[len] == '\'' || s[len] == '"'))
			*quote = s[len];
		else if (*quote && s[len] == *quote)
			*quote = 0;
		len++;
	}
	return (len);
}

t_tok	*lex_tokens(char *s)
{
	t_tok	*list;
	t_tok	*last;
	int		i;
	int		start_i;

	list = NULL;
	i = 0;
	while (s[i])
	{
		start_i = i;
		while (s[i] && ft_strchr(" \t\n", s[i]))
			i++;
		if (list && i == start_i)
		{
			last = list;
			while (last->next)
				last = last->next;
			last->join_next = true;
		}
		if (!s[i])
			break ;
		if (s[i] == '#' && (i == 0 || ft_strchr(" \t", s[i - 1])))
			break ;
		if (is_meta(s[i]))
			i = lex_op(s, i, &list);
		else
			i = lex_word(s, i, &list);
	}
	tok_add(&list, new_tok(TOK_END, NULL, i));
	return (list);
}
