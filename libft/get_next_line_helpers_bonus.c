/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_helpers_bonus.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:10:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:05:57 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	stash_append(t_fd_node *node, char *buffer,
		t_fd_node **fd_list, int fd)
{
	char	*temp;

	if (!node->stash)
		temp = ft_strdup(buffer);
	else
		temp = ft_strjoin(node->stash, buffer);
	if (!temp)
		return (remove_fd_node(fd_list, fd), 0);
	free(node->stash);
	node->stash = temp;
	return (1);
}

int	read_to_stash(int fd, t_fd_node *node, t_fd_node **fd_list,
		char *buffer)
{
	ssize_t	rd;

	rd = 1;
	while (!(node->stash && ft_strchr(node->stash, '\n')) && rd > 0)
	{
		rd = read(fd, buffer, BUFFER_SIZE);
		if (rd <= 0)
			break ;
		buffer[rd] = '\0';
		if (!stash_append(node, buffer, fd_list, fd))
			return (-1);
	}
	if (rd < 0)
		return (0);
	return (1);
}
