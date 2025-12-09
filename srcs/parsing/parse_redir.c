/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 18:30:02 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/12/08 18:30:05 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir_type	redir_type_from_tok(t_tok_type type)
{
	if (type == TOK_IN)
		return (R_IN);
	if (type == TOK_OUT)
		return (R_OUT);
	if (type == TOK_APP)
		return (R_APP);
	return (R_HEREDOC);
}

static bool	add_redir_target(t_tok **cur, t_redir_type type, t_redir **redirs)
{
	t_tok		*target;
	t_redir		*tmp;

	target = *cur;
	if (!target || target->type != TOK_WORD)
	{
		syntax_err_tok(target);
		return (false);
	}
	if (!*redirs)
		*redirs = new_redir(type, target->value);
	else
	{
		tmp = *redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_redir(type, target->value);
	}
	*cur = target->next;
	return (true);
}

bool	parse_one_redir(t_tok **cur, t_redir **redirs)
{
	t_tok			*op;
	t_redir_type	type;

	op = *cur;
	if (!op || (op->type != TOK_IN && op->type != TOK_OUT
			&& op->type != TOK_APP && op->type != TOK_HEREDOC))
		return (false);
	type = redir_type_from_tok(op->type);
	*cur = op->next;
	return (add_redir_target(cur, type, redirs));
}
