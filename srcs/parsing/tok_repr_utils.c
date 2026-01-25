/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_repr_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:10:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:05:57 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

const char	*tok_repr_op(t_tok_type type)
{
	if (type == TOK_PIPE)
		return ("|");
	if (type == TOK_PIPE_AMP)
		return ("|&");
	if (type == TOK_AND)
		return ("&&");
	if (type == TOK_OR)
		return ("||");
	if (type == TOK_LPAREN)
		return ("(");
	if (type == TOK_RPAREN)
		return (")");
	if (type == TOK_IN)
		return ("<");
	if (type == TOK_OUT)
		return (">");
	if (type == TOK_APP)
		return (">>");
	if (type == TOK_HEREDOC)
		return ("<<");
	if (type == TOK_AMP)
		return ("&");
	if (type == TOK_SEMI)
		return (";");
	return (NULL);
}

const char	*tok_repr(t_tok *tok)
{
	const char	*repr;

	if (!tok || tok->type == TOK_END)
		return ("newline");
	if (tok->type == TOK_WORD && tok->value)
		return (tok->value);
	repr = tok_repr_op(tok->type);
	if (repr)
		return (repr);
	return ("token");
}
