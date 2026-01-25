/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 06:45:24 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/08/13 01:20:02 by dayana           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	remove_fd_node(t_fd_node **head, int fd)
{
	t_fd_node	*cur;
	t_fd_node	*prev;

	if (!head || !*head)
		return ;
	cur = *head;
	prev = NULL;
	while (cur)
	{
		if (cur->fd == fd)
		{
			if (prev)
				prev->next = cur->next;
			else
				*head = cur->next;
			free (cur->stash);
			free (cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}
