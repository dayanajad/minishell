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

char	*expand_ansi_c_quote(char *s, int *i);
char	*process_backslashes(char *s, int *i, bool sq, bool dq);

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

static char	*handle_dollar_quote(char *s, int *i, t_chunk_ctx *ctx)
{
	if (!ctx->sq && s[*i] == '$' && s[*i + 1] == '\'')
		return (expand_ansi_c_quote(s, i));
	if (s[*i] == '$' && !ctx->sq && !ctx->dq && s[*i + 1] == '"')
	{
		(*i)++;
		return (ft_strdup(""));
	}
	return (NULL);
}

char	*get_next_chunk(char *s, int *i, t_chunk_ctx *ctx)
{
	char	*res;

	if (s[*i] == '\\' && !ctx->sq)
		return (process_backslashes(s, i, ctx->sq, ctx->dq));
	res = handle_dollar_quote(s, i, ctx);
	if (res)
		return (res);
	if (s[*i] == '$' && !ctx->sq && (ft_isalnum(s[*i + 1])
			|| s[*i + 1] == '_' || s[*i + 1] == '?'))
		return (expand_var(s, i, ctx->sh));
	return (process_char(s[(*i)++]));
}

static char	*process_heredoc_char(char *s, int *i, t_shell *shell)
{
	char	*val;

	if (s[*i] == '$' && (ft_isalnum(s[*i + 1])
			|| s[*i + 1] == '_' || s[*i + 1] == '?'))
		val = expand_var(s, i, shell);
	else
		val = process_char(s[(*i)++]);
	return (val);
}

char	*expand_heredoc_str(char *s, t_shell *shell)
{
	char	*res;
	char	*val;
	int		i;

	if (!s)
		return (NULL);
	res = ft_strdup("");
	i = 0;
	while (s[i])
	{
		val = process_heredoc_char(s, &i, shell);
		res = append_val(res, val);
		free(val);
	}
	free(s);
	return (res);
}
