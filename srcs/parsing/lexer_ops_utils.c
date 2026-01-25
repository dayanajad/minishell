/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_ops_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:00:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/26 12:00:00 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	lex_pipe_op(char *s, int i, t_tok **list)
{
	if (s[i + 1] == '&')
	{
		tok_add(list, new_tok(TOK_PIPE_AMP, NULL, i));
		return (i + 2);
	}
	if (s[i + 1] == '|')
	{
		tok_add(list, new_tok(TOK_OR, NULL, i));
		return (i + 2);
	}
	tok_add(list, new_tok(TOK_PIPE, NULL, i));
	return (i + 1);
}

int	lex_amp_op(char *s, int i, t_tok **list)
{
	if (s[i + 1] == '&')
	{
		tok_add(list, new_tok(TOK_AND, NULL, i));
		return (i + 2);
	}
	tok_add(list, new_tok(TOK_AMP, NULL, i));
	return (i + 1);
}
