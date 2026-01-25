/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_resolve.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:00:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/26 12:00:00 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_direct_path(const char *path)
{
	struct stat	st;

	if (stat(path, &st) < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(path);
		exit(127);
	}
	if (S_ISDIR(st.st_mode))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		exit(126);
	}
	if (access(path, X_OK) < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(path);
		exit(126);
	}
}

static void	check_path_access(char *path)
{
	if (access(path, X_OK) < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(path);
		free(path);
		exit(126);
	}
}

char	*resolve_cmd_path(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	path = find_in_path(cmd->av[0], shell->env);
	if (!path)
	{
		exec_print_cmd_not_found(shell, cmd->av[0]);
		exit(127);
	}
	if (ft_strchr(cmd->av[0], '/'))
		check_direct_path(path);
	else
		check_path_access(path);
	return (path);
}
