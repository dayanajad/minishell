/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:10:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 18:44:40 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	insert_matches(t_tok **cur, char **matches)
{
	t_tok	*next;
	t_tok	*new;
	int		i;

	next = (*cur)->next;
	free((*cur)->value);
	(*cur)->value = ft_strdup(matches[0]);
	(*cur)->has_wildcard = false;
	(*cur)->from_glob = true;
	i = 1;
	while (matches[i])
	{
		new = new_tok(TOK_WORD, ft_strdup(matches[i]), 0);
		new->from_glob = true;
		new->next = next;
		(*cur)->next = new;
		*cur = new;
		i++;
	}
}
