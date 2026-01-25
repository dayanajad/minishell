/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_line_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:00:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/26 12:00:00 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	get_open_quote(char *line)
{
	char	quote;

	quote = 0;
	scan_to_comment_or_eof(line, &quote);
	return (quote);
}

char	*join_piece(char *acc, char *piece, bool insert_newline)
{
	char	*tmp;

	if (!acc)
		return (piece);
	if (insert_newline)
	{
		tmp = ft_strjoin(acc, "\n");
		free(acc);
		if (!tmp)
		{
			free(piece);
			return (NULL);
		}
		acc = tmp;
	}
	tmp = ft_strjoin(acc, piece);
	free(acc);
	free(piece);
	return (tmp);
}

char	*handle_multiline_eof(char *line, char quote)
{
	ft_putstr_fd("minishell: unexpected EOF while looking ", 2);
	ft_putstr_fd("for matching `", 2);
	ft_putchar_fd(quote, 2);
	ft_putendl_fd("'", 2);
	free(line);
	return (NULL);
}
