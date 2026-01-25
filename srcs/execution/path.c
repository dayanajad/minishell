/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 18:48:44 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/07 00:39:53 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*build_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*path;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	path = ft_strjoin(tmp, cmd);
	free(tmp);
	return (path);
}

static char	*search_in_dirs(char **dirs, char *cmd, bool is_fallback)
{
	char		*path;
	char		*found_noexec;
	int			i;
	struct stat	st;

	found_noexec = NULL;
	i = 0;
	while (dirs[i])
	{
		path = build_path(dirs[i], cmd);
		if (!path)
			return (found_noexec);
		if (stat(path, &st) == 0 && !S_ISDIR(st.st_mode))
		{
			if (access(path, X_OK) == 0)
			{
				free_str_arr(dirs);
				free(found_noexec);
				return (path);
			}
			if (is_fallback && !found_noexec)
				found_noexec = ft_strdup(path);
		}
		free(path);
		i++;
	}
	free_str_arr(dirs);
	return (found_noexec);
}

char	*find_in_path(char *cmd, t_env *env)
{
	char	*path_env;
	char	**dirs;
	char	**fallback;

	if (!cmd || !cmd[0])
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		return (ft_strdup(cmd));
	}
	path_env = get_env_value(env, "PATH");
	if (!path_env)
	{
		fallback = ft_calloc(2, sizeof(*fallback));
		if (!fallback)
			return (NULL);
		fallback[0] = ft_strdup(".");
		if (!fallback[0])
		{
			free(fallback);
			return (NULL);
		}
		return (search_in_dirs(fallback, cmd, true));
	}
	dirs = ft_split(path_env, ':');
	if (!dirs)
		return (NULL);
	return (search_in_dirs(dirs, cmd, false));
}
