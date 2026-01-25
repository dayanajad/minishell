/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:10:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:05:57 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_op_token(t_tok_type type)
{
	return (type == TOK_PIPE || type == TOK_AND || type == TOK_OR
		|| type == TOK_AMP || type == TOK_PIPE_AMP || type == TOK_SEMI);
}

bool	is_redir_token(t_tok_type type)
{
	return (type == TOK_IN || type == TOK_OUT
		|| type == TOK_APP || type == TOK_HEREDOC);
}

bool	check_op_seq(t_tok *cur)
{
	if (!is_op_token(cur->type))
		return (true);
	if (cur->type == TOK_SEMI)
	{
		if (!cur->next || cur->next->type == TOK_END)
			return (true);
		if (cur->next->type == TOK_SEMI)
		{
			syntax_err_tok(cur->next);
			return (false);
		}
		if (is_op_token(cur->next->type))
		{
			syntax_err_tok(cur->next);
			return (false);
		}
		return (true);
	}
	if (cur->type == TOK_PIPE_AMP)
	{
		syntax_err_tok(cur);
		return (false);
	}
	if (!cur->next || is_cmd_end(cur->next))
	{
		if (cur->type == TOK_AMP
			&& (!cur->next || cur->next->type == TOK_END))
			return (true);
		if (cur->next && cur->next->type == TOK_END)
		{
			ft_putstr_fd("minishell: syntax error: ", 2);
			ft_putstr_fd("unexpected end of file\n", 2);
			return (false);
		}
		syntax_err_tok(cur->next);
		return (false);
	}
	return (true);
}

bool	check_redir_seq(t_tok *cur)
{
	if (!is_redir_token(cur->type))
		return (true);
	if (cur->next && cur->next->type == TOK_WORD)
		return (true);
	if (cur->type == TOK_HEREDOC && cur->next
		&& cur->next->type == TOK_IN
		&& (!cur->next->next || cur->next->next->type == TOK_END))
	{
		ft_putstr_fd("minishell: syntax error near ", 2);
		ft_putstr_fd("unexpected token `newline'\n", 2);
		return (false);
	}
	if (cur->type == TOK_HEREDOC && cur->next
		&& cur->next->type == TOK_HEREDOC)
	{
		ft_putstr_fd("minishell: syntax error near ", 2);
		ft_putstr_fd("unexpected token `<'\n", 2);
		return (false);
	}
	syntax_err_tok(cur->next);
	return (false);
}
