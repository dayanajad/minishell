/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:54:10 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/12 22:16:04 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	lex_word(char *s, int i, t_tok **list)
{
	int		start;
	char	quote;
	char	*word;

	start = i;
	while (s[i] && !is_meta(s[i]))
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			quote = s[i++];
			while (s[i] && s[i] != quote)
				i++;
			if (s[i])
				i++;
		}
		else
			i++;
	}
	word = ft_substr(s, start, i - start);
	tok_add(list, new_tok(TOK_WORD, word));
	return (i);
}

static int	lex_redir_op(char *s, int i, t_tok **list)
{
	if (s[i] == '<')
	{
		if (s[i + 1] == '<')
		{
			tok_add(list, new_tok(TOK_HEREDOC, NULL));
			return (i + 2);
		}
		tok_add(list, new_tok(TOK_IN, NULL));
		return (i + 1);
	}
	if (s[i] == '>')
	{
		if (s[i + 1] == '>')
		{
			tok_add(list, new_tok(TOK_APP, NULL));
			return (i + 2);
		}
		tok_add(list, new_tok(TOK_OUT, NULL));
		return (i + 1);
	}
	return (i);
}

static int	lex_misc_op(char *s, int i, t_tok **list)
{
	if (s[i] == '(')
	{
		tok_add(list, new_tok(TOK_LPAREN, NULL));
		return (i + 1);
	}
	if (s[i] == ')')
	{
		tok_add(list, new_tok(TOK_RPAREN, NULL));
		return (i + 1);
	}
	return (i + 1);
}

static int	lex_op(char *s, int i, t_tok **list)
{
	if (s[i] == '|')
	{
		if (s[i + 1] == '|')
		{
			tok_add(list, new_tok(TOK_OR, NULL));
			return (i + 2);
		}
		tok_add(list, new_tok(TOK_PIPE, NULL));
		return (i + 1);
	}
	if (s[i] == '&' && s[i + 1] == '&')
	{
		tok_add(list, new_tok(TOK_AND, NULL));
		return (i + 2);
	}
	if (ft_strchr("<>", s[i]))
		return (lex_redir_op(s, i, list));
	return (lex_misc_op(s, i, list));
}

t_tok	*lexer(char *s)
{
	t_tok	*list;
	int		i;

	list = NULL;
	i = 0;
	while (s[i])
	{
		while (s[i] && ft_strchr(" \t\n", s[i]))
			i++;
		if (!s[i])
			break ;
		if (is_meta(s[i]))
			i = lex_op(s, i, &list);
		else
			i = lex_word(s, i, &list);
	}
	tok_add(&list, new_tok(TOK_END, NULL));
	return (list);
}
