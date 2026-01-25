/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:10:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:05:57 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	resolve_redir_target(t_redir *r, char **target, char ***matches)
{
	*matches = NULL;
	*target = r->target;
	if (ft_strchr(*target, '*') && r->type != R_HEREDOC)
	{
		*matches = get_matches(*target);
		if (*matches && (*matches)[0] && (*matches)[1])
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(*target, 2);
			ft_putendl_fd(": ambiguous redirect", 2);
			free_str_arr(*matches);
			return (false);
		}
		if (*matches && (*matches)[0])
			*target = (*matches)[0];
	}
	return (true);
}

bool	dup_and_close(int fd, int dest)
{
	if (dup2(fd, dest) < 0)
	{
		perror("minishell: dup2");
		close(fd);
		return (false);
	}
	close(fd);
	return (true);
}
