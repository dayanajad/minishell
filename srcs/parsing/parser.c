/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 17:10:43 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/12/08 17:10:46 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*parse_pipe(t_tok **cur, t_shell *shell)
{
	t_ast	*left;
	t_tok	*tok;
	t_ast	*right;

	left = parse_subshell(cur, shell);
	if (!left)
		return (NULL);
	tok = *cur;
	while (tok && tok->type == TOK_PIPE)
	{
		*cur = tok->next;
		if (is_cmd_end(*cur))
		{
			syntax_err_tok(*cur);
			free_ast(left);
			return (NULL);
		}
		right = parse_subshell(cur, shell);
		if (!right)
			return (parse_err(left));
		left = new_ast_bin(A_PIPE, left, right);
		tok = *cur;
	}
	return (left);
}

t_ast	*parse_and(t_tok **cur, t_shell *shell)
{
	t_ast	*left;
	t_tok	*tok;
	t_ast	*right;

	left = parse_pipe(cur, shell);
	if (!left)
		return (NULL);
	tok = *cur;
	while (tok && tok->type == TOK_AND)
	{
		*cur = tok->next;
		if (is_cmd_end(*cur))
		{
			syntax_err_tok(*cur);
			free_ast(left);
			return (NULL);
		}
		right = parse_pipe(cur, shell);
		if (!right)
			return (parse_err(left));
		left = new_ast_bin(A_AND, left, right);
		tok = *cur;
	}
	return (left);
}

t_ast	*parse_or(t_tok **cur, t_shell *shell)
{
	t_ast	*left;
	t_tok	*tok;

	left = parse_and(cur, shell);
	if (!left)
		return (NULL);
	tok = *cur;
	while (tok && (tok->type == TOK_OR || tok->type == TOK_AMP))
	{
		*cur = tok->next;
		if (tok->type == TOK_OR)
			left = parse_or_token(cur, shell, left);
		else
			left = parse_amp_token(cur, shell, left);
		if (!left)
			return (NULL);
		tok = *cur;
	}
	return (left);
}

static t_ast	*parse_seq(t_tok **cur, t_shell *shell)
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
			free_ast(left);
			return (NULL);
		}
		right = parse_or(cur, shell);
		if (!right)
			return (parse_err(left));
		left = new_ast_bin(A_SEQ, left, right);
		tok = *cur;
	}
	return (left);
}

t_ast	*parse(t_tok *tokens, t_shell *shell)
{
	t_tok	*cur;
	t_ast	*root;

	if (!tokens)
		return (NULL);
	cur = tokens;
	root = parse_seq(&cur, shell);
	if (!root)
		return (NULL);
	if (cur && cur->type != TOK_END)
	{
		syntax_err_tok(cur);
		free_ast(root);
		return (NULL);
	}
	return (root);
}
