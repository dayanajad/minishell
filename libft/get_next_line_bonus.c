/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 06:48:02 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/08/13 01:20:37 by dayana           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_fd_node	*get_fd_node(t_fd_node **head, int fd)
{
	t_fd_node	*cur;

	cur = *head;
	while (cur)
	{
		if (cur->fd == fd)
			return (cur);
		cur = cur->next;
	}
	cur = malloc(sizeof(t_fd_node));
	if (!cur)
		return (NULL);
	cur->fd = fd;
	cur->stash = NULL;
	cur->next = *head;
	*head = cur;
	return (cur);
}

static int	read_and_stash(int fd, t_fd_node *node, t_fd_node **fd_list)
{
	char	*buffer;
	int		res;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (0);
	res = read_to_stash(fd, node, fd_list, buffer);
	free(buffer);
	if (res <= 0 || !node->stash || !*(node->stash))
		return (remove_fd_node(fd_list, fd), 0);
	return (1);
}

char	*get_next_line(int fd)
{
	static t_fd_node	*fd_list;
	t_fd_node			*node;
	char				*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	node = get_fd_node(&fd_list, fd);
	if (!node)
		return (NULL);
	if (!read_and_stash(fd, node, &fd_list))
		return (NULL);
	line = extract_line(node->stash);
	node->stash = update_stash(node->stash);
	if (!node->stash)
		remove_fd_node(&fd_list, fd);
	return (line);
}
