/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:00:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/26 12:00:00 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_semi_seq(t_tok *cur)
{
	if (!cur->next || cur->next->type == TOK_END)
		return (true);
	if (cur->next->type == TOK_SEMI || is_op_token(cur->next->type))
	{
		syntax_err_tok(cur->next);
		return (false);
	}
	return (true);
}

bool	check_op_end(t_tok *cur)
{
	if (cur->type == TOK_AMP && (!cur->next || cur->next->type == TOK_END))
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
