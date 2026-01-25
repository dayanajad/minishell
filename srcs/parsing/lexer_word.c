/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:10:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 18:44:40 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	scan_ansi_c_quote(char *s, int i);
int	scan_locale_quote(char *s, int i);

static int	scan_quoted(char *s, int i, bool *was_quoted)
{
	char	quote;

	*was_quoted = true;
	quote = s[i++];
	while (s[i] && s[i] != quote)
		i++;
	if (s[i])
		i++;
	return (i);
}

static int	scan_unquoted(char *s, int i, bool *has_wildcard)
{
	while (s[i])
	{
		if (is_meta(s[i]))
			break ;
		if (s[i] == '$' && (s[i + 1] == '\'' || s[i + 1] == '"'))
			break ;
		if (s[i] == '\'' || s[i] == '"')
			break ;
		if (s[i] == '*')
			*has_wildcard = true;
		i++;
	}
	return (i);
}

static int	scan_word(char *s, int i, bool *has_wildcard, bool *was_quoted)
{
	if (s[i] == '$' && s[i + 1] == '\'')
	{
		*was_quoted = true;
		return (scan_ansi_c_quote(s, i));
	}
	if (s[i] == '$' && s[i + 1] == '"')
	{
		*was_quoted = true;
		return (scan_locale_quote(s, i));
	}
	if (s[i] == '\'' || s[i] == '"')
		return (scan_quoted(s, i, was_quoted));
	*was_quoted = false;
	return (scan_unquoted(s, i, has_wildcard));
}

int	lex_word(char *s, int i, t_tok **list)
{
	int		start;
	bool	has_wildcard;
	bool	was_quoted;
	t_tok	*tok;

	start = i;
	has_wildcard = false;
	was_quoted = false;
	i = scan_word(s, i, &has_wildcard, &was_quoted);
	tok = new_tok(TOK_WORD, ft_substr(s, start, i - start), start);
	tok->has_wildcard = has_wildcard;
	tok->was_quoted = was_quoted;
	tok_add(list, tok);
	return (i);
}
