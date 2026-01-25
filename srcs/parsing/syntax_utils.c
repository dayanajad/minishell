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

bool	check_semi_seq(t_tok *cur);
bool	check_op_end(t_tok *cur);

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
		return (check_semi_seq(cur));
	if (cur->type == TOK_PIPE_AMP)
	{
		syntax_err_tok(cur);
		return (false);
	}
	if (!cur->next || is_cmd_end(cur->next))
		return (check_op_end(cur));
	return (true);
}

static bool	check_heredoc_edge_cases(t_tok *cur)
{
	if (cur->type == TOK_HEREDOC && cur->next && cur->next->type == TOK_IN
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
	return (true);
}

bool	check_redir_seq(t_tok *cur)
{
	if (!is_redir_token(cur->type))
		return (true);
	if (cur->next && cur->next->type == TOK_WORD)
		return (true);
	if (!check_heredoc_edge_cases(cur))
		return (false);
	syntax_err_tok(cur->next);
	return (false);
}
