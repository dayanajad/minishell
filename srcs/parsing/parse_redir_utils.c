/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:10:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:05:57 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*unescape_backslashes(const char *s)
{
	char	*out;
	size_t	i;
	size_t	j;
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	out = (char *)malloc(len + 1);
	if (!out)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] == '\\' && s[i + 1])
		{
			out[j++] = s[i + 1];
			i += 2;
			continue ;
		}
		out[j++] = s[i++];
	}
	out[j] = '\0';
	return (out);
}

void	append_redir(t_redir **redirs, t_redir *new)
{
	t_redir	*tmp;

	if (!new)
		return ;
	if (!*redirs)
	{
		*redirs = new;
		return ;
	}
	tmp = *redirs;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

bool	is_all_digits(const char *s)
{
	int	i;

	if (!s || !*s)
		return (false);
	i = 0;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (false);
		i++;
	}
	return (true);
}
