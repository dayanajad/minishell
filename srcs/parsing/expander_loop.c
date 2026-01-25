/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:00:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/26 12:00:00 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	word_has_wildcard(const char *s)
{
	if (!s)
		return (false);
	return (ft_strchr(s, '*') != NULL);
}

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

static void	process_word_tok(t_tok *cur, t_tok *prev, t_shell *shell)
{
	if (prev && prev->type == TOK_HEREDOC)
		return ;
	expand_tilde_tok(cur, shell);
	cur->value = expand_str(cur->value, shell);
	if (!cur->was_quoted)
		cur->has_wildcard = word_has_wildcard(cur->value);
}

void	expand_toks_loop(t_tok **tokens, t_shell *shell)
{
	t_tok	*cur;
	t_tok	*prev;
	t_tok	*next;

	cur = *tokens;
	prev = NULL;
	while (cur)
	{
		next = cur->next;
		if (cur->type == TOK_WORD)
			process_word_tok(cur, prev, shell);
		prev = cur;
		cur = next;
	}
}
