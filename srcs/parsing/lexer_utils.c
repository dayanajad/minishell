/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:54:10 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/12 22:16:04 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tok	*new_tok(t_tok_type type, char *value, int pos)
{
	t_tok	*tok;

	tok = malloc(sizeof(t_tok));
	if (!tok)
		return (NULL);
	tok->type = type;
	tok->value = value;
	tok->has_wildcard = false;
	tok->was_quoted = false;
	tok->pos = pos;
	tok->join_next = false;
	tok->next = NULL;
	return (tok);
}

void	tok_add(t_tok **head, t_tok *new)
{
	t_tok	*cur;

	if (!*head)
	{
		*head = new;
		return ;
	}
	cur = *head;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

int	is_meta(char c)
{
	return (ft_strchr("|&()<>; \t\n", c) != NULL);
}
