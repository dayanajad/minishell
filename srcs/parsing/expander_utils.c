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
