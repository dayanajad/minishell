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

static void	expand_tilde_tok(t_tok *tok, t_shell *shell)
{
	char	*home;
	char	*joined;

	if (!tok || tok->type != TOK_WORD || tok->was_quoted)
		return ;
	if (!tok->value || tok->value[0] != '~')
		return ;
	if (tok->value[1] && tok->value[1] != '/')
		return ;
	home = get_env_value(shell->env, "HOME");
	if (!home)
		return ;
	joined = ft_strjoin(home, tok->value + 1);
	if (!joined)
		return ;
	free(tok->value);
	tok->value = joined;
}

static char	*whitespace_to_space(const char *s)
{
	char	*tmp;
	int		i;

	if (!s)
		return (NULL);
	tmp = ft_strdup(s);
	if (!tmp)
		return (NULL);
	i = 0;
	while (tmp[i])
	{
		if (tmp[i] == '\t' || tmp[i] == '\n')
			tmp[i] = ' ';
		i++;
	}
	return (tmp);
}

static bool	has_ifs_space(const char *s)
{
	int	i;

	if (!s)
		return (false);
	i = 0;
	while (s[i])
	{
		if (s[i] == ' ' || s[i] == '\t' || s[i] == '\n')
			return (true);
		i++;
	}
	return (false);
}

static bool	word_has_wildcard(const char *s)
{
	if (!s)
		return (false);
	return (ft_strchr(s, '*') != NULL);
}

static t_tok	*split_word_tok(t_tok **tokens, t_tok *prev, t_tok *cur)
{
	char	**parts;
	char	*tmp;
	t_tok	*next;
	t_tok	*last;
	char	*dup;
	t_tok	*new;
	int		i;

	next = cur->next;
	tmp = whitespace_to_space(cur->value);
	if (!tmp)
		return (cur);
	parts = ft_split(tmp, ' ');
	free(tmp);
	if (!parts || !parts[0])
	{
		free_str_arr(parts);
		return (cur);
	}
	free(cur->value);
	cur->value = ft_strdup(parts[0]);
	cur->has_wildcard = word_has_wildcard(cur->value);
	last = cur;
	i = 1;
	while (parts[i])
	{
		dup = ft_strdup(parts[i]);
		if (!dup)
			break ;
		new = new_tok(TOK_WORD, dup);
		if (!new)
		{
			free(dup);
			break ;
		}
		last->next = new;
		last = new;
		last->has_wildcard = word_has_wildcard(last->value);
		last->was_quoted = false;
		i++;
	}
	last->next = next;
	free_str_arr(parts);
	(void)tokens;
	(void)prev;
	return (last);
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
		val = get_next_chunk(s, &i, shell, sq, dq);
		res = append_val(res, val);
		free(val);
	}
	free((void *)s);
	return (res);
}

static t_tok	*remove_empty_word(t_tok **tokens, t_tok *prev,
		t_tok *cur, t_tok *next)
{
	if (prev)
		prev->next = next;
	else
		*tokens = next;
	free(cur->value);
	free(cur);
	return (next);
}

void	expand_tokens(t_tok **tokens, t_shell *shell)
{
	t_tok	*cur;
	t_tok	*prev;
	t_tok	*next;
	t_tok	*last;

	cur = *tokens;
	prev = NULL;
	while (cur)
	{
		next = cur->next;
		if (cur->type == TOK_WORD)
		{
			if (prev && prev->type == TOK_HEREDOC)
			{
				prev = cur;
				cur = next;
				continue ;
			}
			expand_tilde_tok(cur, shell);
			cur->value = expand_str(cur->value, shell);
			if (cur->value && cur->value[0] == '\0' && !cur->was_quoted)
			{
				cur = remove_empty_word(tokens, prev, cur, next);
				continue ;
			}
			if (!cur->was_quoted && has_ifs_space(cur->value))
			{
				last = split_word_tok(tokens, prev, cur);
				prev = last;
				cur = last->next;
				continue ;
			}
		}
		prev = cur;
		cur = next;
	}
}
