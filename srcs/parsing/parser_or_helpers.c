/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_or_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:10:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:05:57 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*parse_or_token(t_tok **cur, t_shell *shell, t_ast *left)
{
	t_ast	*right;

	if (is_cmd_end(*cur))
	{
		syntax_err_tok(*cur);
		free_ast(left);
		return (NULL);
	}
	right = parse_and(cur, shell);
	if (!right)
		return (parse_err(left));
	return (new_ast_bin(A_OR, left, right));
}

t_ast	*parse_amp_token(t_tok **cur, t_shell *shell, t_ast *left)
{
	t_ast	*right;

	if (!*cur || (*cur)->type == TOK_END)
		return (new_ast_bin(A_BG, left, NULL));
	right = parse_and(cur, shell);
	if (!right)
		return (parse_err(left));
	return (new_ast_bin(A_BG, left, right));
}
