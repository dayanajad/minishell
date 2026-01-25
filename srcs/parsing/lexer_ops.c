/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_ops.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:10:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:05:57 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	lex_redir_op(char *s, int i, t_tok **list)
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
		if (s[i + 1] == '|')
		{
			tok_add(list, new_tok(TOK_OUT, NULL));
			return (i + 2);
		}
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

int	lex_misc_op(char *s, int i, t_tok **list)
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
	if (s[i] == ';')
	{
		tok_add(list, new_tok(TOK_SEMI, NULL));
		return (i + 1);
	}
	return (i + 1);
}

static int	lex_pipe_op(char *s, int i, t_tok **list)
{
	if (s[i + 1] == '&')
	{
		tok_add(list, new_tok(TOK_PIPE_AMP, NULL));
		return (i + 2);
	}
	if (s[i + 1] == '|')
	{
		tok_add(list, new_tok(TOK_OR, NULL));
		return (i + 2);
	}
	tok_add(list, new_tok(TOK_PIPE, NULL));
	return (i + 1);
}

static int	lex_amp_op(char *s, int i, t_tok **list)
{
	if (s[i + 1] == '&')
	{
		tok_add(list, new_tok(TOK_AND, NULL));
		return (i + 2);
	}
	tok_add(list, new_tok(TOK_AMP, NULL));
	return (i + 1);
}

int	lex_op(char *s, int i, t_tok **list)
{
	if (s[i] == '|')
		return (lex_pipe_op(s, i, list));
	if (s[i] == '&')
		return (lex_amp_op(s, i, list));
	if (ft_strchr("<>", s[i]))
		return (lex_redir_op(s, i, list));
	return (lex_misc_op(s, i, list));
}
