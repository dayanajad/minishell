/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_seq.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:00:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/26 12:00:00 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_ast	*parse_and_or_op(t_tok **cur, t_shell *shell, t_ast *left,
		t_tok_type type)
{
	t_ast	*right;

	*cur = (*cur)->next;
	if (is_cmd_end(*cur))
	{
		syntax_err_tok(*cur);
		free_ast(left);
		return (NULL);
	}
	right = parse_pipe(cur, shell);
	if (!right)
		return (parse_err(left));
	if (type == TOK_AND)
		return (new_ast_bin(A_AND, left, right));
	return (new_ast_bin(A_OR, left, right));
}

t_ast	*parse_or(t_tok **cur, t_shell *shell)
{
	t_ast		*left;
	t_tok		*tok;
	t_tok_type	type;

	left = parse_pipe(cur, shell);
	if (!left)
		return (NULL);
	tok = *cur;
	while (tok && (tok->type == TOK_OR || tok->type == TOK_AND
			|| tok->type == TOK_AMP))
	{
		type = tok->type;
		if (type == TOK_AMP)
		{
			*cur = tok->next;
			left = parse_amp_token(cur, shell, left);
		}
		else
			left = parse_and_or_op(cur, shell, left, type);
		if (!left)
			return (NULL);
		tok = *cur;
	}
	return (left);
}

t_ast	*parse_seq(t_tok **cur, t_shell *shell)
{
	t_ast	*left;
	t_tok	*tok;
	t_ast	*right;

	left = parse_or(cur, shell);
	if (!left)
		return (NULL);
	tok = *cur;
	while (tok && tok->type == TOK_SEMI)
	{
		*cur = tok->next;
		if (!*cur || (*cur)->type == TOK_END)
			return (left);
		if (is_cmd_end(*cur))
		{
			syntax_err_tok(*cur);
			return (parse_err(left));
		}
		right = parse_or(cur, shell);
		if (!right)
			return (parse_err(left));
		left = new_ast_bin(A_SEQ, left, right);
		tok = *cur;
	}
	return (left);
}
