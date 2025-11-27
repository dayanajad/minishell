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

static char	*extract_line(char *stash)
{
	char	*line;
	size_t	i;

	i = 0;
	if (!stash || !(*stash))
		return (NULL);
	while (stash[i] && stash[i] != '\n')
		i++;
	line = malloc(i + (stash[i] == '\n') + 1);
	if (!line)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
	{
		line[i] = stash[i];
		i++;
	}
	if (stash[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

static char	*update_stash(char *stash)
{
	char	*new;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!stash)
		return (NULL);
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
		return (free(stash), NULL);
	new = malloc(ft_strlen(stash + i + 1) + 1);
	if (!new)
		return (free (stash), NULL);
	i++;
	while (stash[i])
		new[j++] = stash[i++];
	new[j] = '\0';
	return (free (stash), new);
}

static int	read_and_stash(int fd, t_fd_node *node, t_fd_node **fd_list)
{
	char	*buffer;
	char	*temp;
	ssize_t	rd;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (0);
	rd = 1;
	while (!(ft_strchr(node->stash, '\n')) && rd > 0)
	{
		rd = read(fd, buffer, BUFFER_SIZE);
		if (rd <= 0)
			break ;
		buffer[rd] = '\0';
		temp = ft_strjoin(node->stash, buffer);
		if (!temp)
			return (free(buffer), remove_fd_node(fd_list, fd), 0);
		free(node->stash);
		node->stash = temp;
	}
	free (buffer);
	if (rd < 0 || !node->stash || !*(node->stash))
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
