/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:54:10 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/12 22:16:04 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	check_token_seq(t_tok *cur)
{
	if (!check_op_seq(cur))
		return (false);
	if (!check_redir_seq(cur))
		return (false);
	return (true);
}

bool	check_syntax(t_tok *tokens)
{
	t_tok	*cur;

	cur = tokens;
	if (cur && (cur->type == TOK_PIPE
			|| cur->type == TOK_AND || cur->type == TOK_OR
			|| cur->type == TOK_AMP || cur->type == TOK_PIPE_AMP
			|| cur->type == TOK_SEMI))
	{
		syntax_err_tok(cur);
		return (false);
	}
	while (cur)
	{
		if (!check_token_seq(cur))
			return (false);
		cur = cur->next;
	}
	return (true);
}
