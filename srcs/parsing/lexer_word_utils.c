/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:00:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/26 12:00:00 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	scan_ansi_c_quote(char *s, int i)
{
	i += 2;
	while (s[i] && s[i] != '\'')
	{
		if (s[i] == '\\' && s[i + 1])
			i++;
		i++;
	}
	if (s[i] == '\'')
		i++;
	return (i);
}

int	scan_locale_quote(char *s, int i)
{
	i += 2;
	while (s[i] && s[i] != '"')
	{
		if (s[i] == '\\' && s[i + 1])
			i++;
		i++;
	}
	if (s[i] == '"')
		i++;
	return (i);
}
