/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:10:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:05:57 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	write_all(int fd, const char *s, size_t len)
{
	ssize_t	w;

	while (len > 0)
	{
		w = write(fd, s, len);
		if (w < 0)
			return (-1);
		s += (size_t)w;
		len -= (size_t)w;
	}
	return (0);
}

int	echo_write_str(const char *s)
{
	if (!s)
		return (0);
	if (write_all(STDOUT_FILENO, s, ft_strlen(s)) < 0)
		return (-1);
	return (0);
}

int	echo_write_ch(char c)
{
	if (write_all(STDOUT_FILENO, &c, 1) < 0)
		return (-1);
	return (0);
}

int	write_echo_args(char **av, int i)
{
	while (av[i])
	{
		if (echo_write_str(av[i]) < 0)
			return (1);
		if (av[i + 1])
		{
			if (echo_write_ch(' ') < 0)
				return (1);
		}
		i++;
	}
	return (0);
}
