/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 02:16:57 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 02:16:57 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tok	*lexer(char *s)
{
	char	quote;
	int		len;

	quote = 0;
	len = scan_to_comment_or_eof(s, &quote);
	if (quote)
	{
		ft_putstr_fd("minishell: unexpected EOF while looking ", 2);
		ft_putstr_fd("for matching `", 2);
		ft_putchar_fd(quote, 2);
		ft_putendl_fd("'", 2);
		return (NULL);
	}
	(void)len;
	return (lex_tokens(s));
}
