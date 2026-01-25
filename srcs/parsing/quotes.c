/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:54:10 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/12 22:16:04 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	copy_unquoted(char *res, char *s)
{
	int		i;
	int		j;
	bool	in_sq;
	bool	in_dq;

	i = 0;
	j = 0;
	in_sq = false;
	in_dq = false;
	while (s[i])
	{
		if (s[i] == '\'' && !in_dq)
		{
			in_sq = !in_sq;
			i++;
		}
		else if (s[i] == '"' && !in_sq)
		{
			in_dq = !in_dq;
			i++;
		}
		else
			res[j++] = s[i++];
	}
	res[j] = '\0';
}

static char	*strip_quotes(char *s)
{
	char	*res;

	if (!s)
		return (NULL);
	res = malloc(ft_strlen(s) + 1);
	if (!res)
		return (NULL);
	copy_unquoted(res, s);
	free(s);
	return (res);
}

void	remove_quotes(t_tok *tokens)
{
	t_tok	*cur;

	cur = tokens;
	while (cur)
	{
		if (cur->type == TOK_WORD && !cur->from_glob)
		{
			cur->value = strip_quotes(cur->value);
		}
		cur = cur->next;
	}
}
