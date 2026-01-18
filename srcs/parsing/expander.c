/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:54:10 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/12 22:16:04 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_var(char *s, int *i, t_shell *shell)
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

static char	*get_next_chunk(char *s, int *i, t_shell *shell, bool sq)
{
	if (s[*i] == '$' && !sq && (ft_isalnum(s[*i + 1])
			|| s[*i + 1] == '_' || s[*i + 1] == '?'))
		return (expand_var(s, i, shell));
	return (process_char(s[(*i)++]));
}

char	*expand_str(char *s, t_shell *shell)
{
	char	*res;
	int		i;
	bool	sq;
	bool	dq;
	char	*val;

	if (!s)
		return (NULL);
	res = ft_strdup("");
	i = 0;
	sq = false;
	dq = false;
	while (s[i])
	{
		handle_quotes(s[i], &sq, &dq);
		val = get_next_chunk(s, &i, shell, sq);
		res = append_val(res, val);
		free(val);
	}
	free((void *)s);
	return (res);
}

void	expand_tokens(t_tok *tokens, t_shell *shell)
{
	t_tok	*cur;

	cur = tokens;
	while (cur)
	{
		if (cur->type == TOK_WORD)
		{
			cur->value = expand_str(cur->value, shell);
		}
		cur = cur->next;
	}
}
