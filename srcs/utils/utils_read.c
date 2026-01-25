/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_read.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:10:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:05:57 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*grow_buf(char *buf, size_t *cap, size_t need)
{
	size_t	new_cap;
	char	*new_buf;

	new_cap = *cap;
	if (new_cap == 0)
		new_cap = 64;
	while (new_cap < need)
		new_cap *= 2;
	new_buf = ft_calloc(new_cap, sizeof(char));
	if (!new_buf)
	{
		free(buf);
		return (NULL);
	}
	if (buf)
		ft_memcpy(new_buf, buf, *cap);
	free(buf);
	*cap = new_cap;
	return (new_buf);
}

static int	append_char(char **buf, size_t *cap, size_t *len, char c)
{
	if (*len + 2 > *cap)
	{
		*buf = grow_buf(*buf, cap, *len + 2);
		if (!*buf)
			return (0);
	}
	(*buf)[(*len)++] = c;
	return (1);
}

static char	*finish_buf(char *buf, size_t *cap, size_t len)
{
	if (len + 1 > *cap)
	{
		buf = grow_buf(buf, cap, len + 1);
		if (!buf)
			return (NULL);
	}
	buf[len] = '\0';
	return (buf);
}

char	*read_line_nobuf(int fd)
{
	char	*buf;
	size_t	cap;
	size_t	len;
	char	c;
	ssize_t	rd;

	buf = NULL;
	cap = 0;
	len = 0;
	rd = 1;
	while (rd > 0)
	{
		rd = read(fd, &c, 1);
		if (rd <= 0)
			break ;
		if (!append_char(&buf, &cap, &len, c))
			return (NULL);
		if (c == '\n')
			break ;
	}
	if (rd <= 0 && len == 0)
		return (NULL);
	return (finish_buf(buf, &cap, len));
}
