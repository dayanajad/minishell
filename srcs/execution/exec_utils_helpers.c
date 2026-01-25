/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:10:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:42:17 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	exec_suppress_error(t_shell *shell)
{
	if (!shell)
		return (false);
	return (shell->heredoc_eof && !isatty(STDIN_FILENO));
}

void	exec_print_cmd_not_found(t_shell *shell, char *cmd)
{
	if (exec_suppress_error(shell))
		return ;
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
}

void	exec_print_is_dir(t_shell *shell, char *path)
{
	if (exec_suppress_error(shell))
		return ;
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
}

void	exec_as_script(char *path, t_cmd *cmd, char **envp)
{
	char	**sh_av;
	int		count;
	int		i;

	count = 0;
	while (cmd->av[count])
		count++;
	sh_av = malloc(sizeof(char *) * (count + 2));
	if (!sh_av)
		return ;
	sh_av[0] = ft_strdup("/bin/sh");
	sh_av[1] = ft_strdup(path);
	i = 1;
	while (cmd->av[i])
	{
		sh_av[i + 1] = ft_strdup(cmd->av[i]);
		i++;
	}
	sh_av[count + 1] = NULL;
	execve(sh_av[0], sh_av, envp);
}

void	exec_error_exit(t_shell *shell, char *path)
{
	if (!exec_suppress_error(shell))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(path);
	}
	if (errno == EACCES)
		exit(126);
	if (errno == ENOENT)
		exit(127);
	exit(126);
}
