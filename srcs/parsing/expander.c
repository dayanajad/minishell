/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:54:10 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:05:57 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_toks_loop(t_tok **tokens, t_shell *shell);
void	split_and_clean_toks(t_tok **tokens, t_shell *shell);
void	join_tokens(t_tok **tokens);

char	*expand_str(char *s, t_shell *shell)
{
	char		*res;
	int			i;
	char		*val;
	t_chunk_ctx	ctx;

	if (!s)
		return (NULL);
	res = ft_strdup("");
	i = 0;
	ctx.sh = shell;
	ctx.sq = false;
	ctx.dq = false;
	while (s[i])
	{
		handle_quotes(s[i], &ctx.sq, &ctx.dq);
		val = get_next_chunk(s, &i, &ctx);
		res = append_val(res, val);
		free(val);
	}
	free((void *)s);
	return (res);
}

void	expand_tokens(t_tok **tokens, t_shell *shell)
{
	expand_toks_loop(tokens, shell);
	split_and_clean_toks(tokens, shell);
	join_tokens(tokens);
}
