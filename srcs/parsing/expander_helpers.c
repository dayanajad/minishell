/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:10:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:05:57 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_hex_digit(char c)
{
	return (ft_isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}

static int	hex_val(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (10 + (c - 'a'));
	if (c >= 'A' && c <= 'F')
		return (10 + (c - 'A'));
	return (0);
}

static char	*ansi_c_escape(char *s, int *i)
{
	char	c;
	char	out;
	int	v;
	int	count;

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
	{
		(*i)++;
		v = 0;
		count = 0;
		while (s[*i] && is_hex_digit(s[*i]) && count < 2)
		{
			v = (v * 16) + hex_val(s[*i]);
			(*i)++;
			count++;
		}
		out = (char)v;
		return (process_char(out));
	}
	if (c >= '0' && c <= '7')
	{
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
	return (process_char(s[(*i)++]));
}

static char	*expand_ansi_c_quote(char *s, int *i)
{
	char	*res;
	char	*val;

	res = ft_strdup("");
	if (!res)
		return (NULL);
	*i += 2;
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
	if (s[*i] == '\'')
		(*i)++;
	return (res);
}

static char	*make_backslashes(int count)
{
	char	*out;
	int		j;

	if (count <= 0)
		return (ft_strdup(""));
	out = ft_calloc(count + 1, sizeof(char));
	if (!out)
		return (NULL);
	j = 0;
	while (j < count)
		out[j++] = '\\';
	return (out);
}

static char	*process_backslashes(char *s, int *i, bool sq, bool dq)
{
	int		count;
	int		literal;
	char	next;
	char	*prefix;
	char	*ch;
	char	*joined;

	if (sq)
		return (process_char(s[(*i)++]));
	count = 0;
	while (s[*i + count] == '\\')
		count++;
	next = s[*i + count];
	if (!next)
	{
		*i += count;
		return (make_backslashes(count));
	}
	if (dq && next != '\\' && next != '"' && next != '$' && next != '\n')
	{
		*i += count;
		return (make_backslashes(count));
	}
	literal = count / 2;
	if (count % 2 == 0)
	{
		*i += count;
		return (make_backslashes(literal));
	}
	*i += count + 1;
	prefix = make_backslashes(literal);
	ch = process_char(next);
	joined = append_val(prefix, ch);
	free(ch);
	return (joined);
}

char	*expand_var(char *s, int *i, t_shell *shell)
{
	int		len;
	char	*key;
	char	*val;

	(*i)++;
	if (s[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(shell->last_status));
	}
	len = 0;
	while (s[*i + len] && (ft_isalnum(s[*i + len]) || s[*i + len] == '_'))
		len++;
	if (len == 0)
		return (ft_strdup("$"));
	key = ft_substr(s, *i, len);
	*i += len;
	val = get_env_value(shell->env, key);
	free(key);
	if (!val)
		return (ft_strdup(""));
	return (ft_strdup(val));
}

char	*get_next_chunk(char *s, int *i, t_shell *shell, bool sq, bool dq)
{
	if (s[*i] == '\\' && !sq)
		return (process_backslashes(s, i, sq, dq));
	if (!sq && s[*i] == '$' && s[*i + 1] == '\'')
		return (expand_ansi_c_quote(s, i));
	if (s[*i] == '$' && !sq && !dq && s[*i + 1] == '"')
	{
		(*i)++;
		return (ft_strdup(""));
	}
	if (s[*i] == '$' && !sq && (ft_isalnum(s[*i + 1])
			|| s[*i + 1] == '_' || s[*i + 1] == '?'))
		return (expand_var(s, i, shell));
	return (process_char(s[(*i)++]));
}
