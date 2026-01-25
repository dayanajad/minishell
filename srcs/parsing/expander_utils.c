/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 02:16:35 by bpichyal          #+#    #+#             */
/*   Updated: 2026/01/25 02:16:35 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*append_val(char *res, char *val)
{
	char	*tmp;

	if (!val)
		return (res);
	if (!res)
		return (ft_strdup(val));
	tmp = ft_strjoin(res, val);
	free(res);
	return (tmp);
}

void	handle_quotes(char c, bool *in_sq, bool *in_dq)
{
	if (c == '\'' && !*in_dq)
		*in_sq = !*in_sq;
	else if (c == '"' && !*in_sq)
		*in_dq = !*in_dq;
}

char	*process_char(char c)
{
	char	str[2];

	str[0] = c;
	str[1] = '\0';
	return (ft_strdup(str));
}

char	*whitespace_to_space(const char *s)
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

bool	has_ifs_space(const char *s)
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

bool	word_has_wildcard(const char *s)
{
	if (!s)
		return (false);
	return (ft_strchr(s, '*') != NULL);
}
