/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_ansi.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:00:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/26 12:00:00 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*escape_hex(char *s, int *i)
{
	int		v;
	int		count;
	char	out;

	v = 0;
	count = 0;
	(*i)++;
	while (s[*i] && ansi_is_hex_digit(s[*i]) && count < 2)
	{
		v = (v * 16) + ansi_hex_val(s[*i]);
		(*i)++;
		count++;
	}
	out = (char)v;
	return (process_char(out));
}

static char	*escape_octal(char *s, int *i)
{
	int		v;
	int		count;
	char	out;

	v = 0;
	count = 0;
	while (s[*i] && s[*i] >= '0' && s[*i] <= '7' && count < 3)
	{
		v = (v * 8) + (s[*i] - '0');
		(*i)++;
		count++;
	}
	out = (char)v;
	return (process_char(out));
}

static char	*ansi_c_escape(char *s, int *i)
{
	char	c;

	c = s[*i];
	if (c == 'a')
		return ((*i)++, process_char('\a'));
	if (c == 'b')
		return ((*i)++, process_char('\b'));
	if (c == 'f')
		return ((*i)++, process_char('\f'));
	if (c == 'n')
		return ((*i)++, process_char('\n'));
	if (c == 'r')
		return ((*i)++, process_char('\r'));
	if (c == 't')
		return ((*i)++, process_char('\t'));
	if (c == 'v')
		return ((*i)++, process_char('\v'));
	if (c == '\\' || c == '\'' || c == '"')
		return ((*i)++, process_char(c));
	if (c == 'x')
		return (escape_hex(s, i));
	if (c >= '0' && c <= '7')
		return (escape_octal(s, i));
	return (process_char(s[(*i)++]));
}

static char	*expand_loop(char *s, int *i, char *res)
{
	char	*val;

	while (s[*i] && s[*i] != '\'')
	{
		if (s[*i] == '\\' && s[*i + 1])
		{
			(*i)++;
			val = ansi_c_escape(s, i);
		}
		else
			val = process_char(s[(*i)++]);
		res = append_val(res, val);
		free(val);
	}
	return (res);
}

char	*expand_ansi_c_quote(char *s, int *i)
{
	char	*res;

	res = ft_strdup("");
	if (!res)
		return (NULL);
	*i += 2;
	res = expand_loop(s, i, res);
	if (s[*i] == '\'')
		(*i)++;
	return (res);
}
