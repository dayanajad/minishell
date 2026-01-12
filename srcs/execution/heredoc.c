/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 21:53:47 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/12 22:08:49 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_temp_file(void)
{
	static int	counter = 0;
	char		*num_str;
	char		*temp_path;
	char		*tmp;

	num_str = ft_itoa(counter++);
	if (!num_str)
		return (NULL);
	tmp = ft_strjoin("/tmp/.minishell_heredoc_", num_str);
	free(num_str);
	if (!tmp)
		return (NULL);
	temp_path = ft_strjoin(tmp, "_XXXXXX");
	free(tmp);
	return (temp_path);
}

static int	open_temp_file(char **path)
{
	int		fd;
	char	*temp_template;

	temp_template = create_temp_file();
	if (!temp_template)
		return (-1);
	fd = mkstemp(temp_template);
	if (fd < 0)
	{
		free(temp_template);
		perror("minishell: mkstemp");
		return (-1);
	}
	*path = temp_template;
	return (fd);
}

char	*read_heredoc(const char *delimiter)
{
	int		fd;
	char	*line;
	char	*temp_path;

	fd = open_temp_file(&temp_path);
	if (fd < 0)
		return (NULL);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	return (temp_path);
}
