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

t_ast	*parse_seq(t_tok **cur, t_shell *shell);

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
			return (parse_err(left));
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
	return (parse_or(cur, shell));
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
