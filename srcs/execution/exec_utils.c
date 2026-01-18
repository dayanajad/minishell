/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 19:06:48 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/06 23:45:18 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	normalize_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

void	exec_external_child(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	char	**envp;

	path = find_in_path(cmd->av[0], shell->env);
	if (!path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->av[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit(127);
	}
	envp = env_to_arr(shell->env);
	if (!envp)
	{
		free(path);
		exit(1);
	}
	execve(path, cmd->av, envp);
	perror("minishell");
	free(path);
	free_str_arr(envp);
	exit(126);
}

int	exec_external_cmd(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
	{
		if (!apply_redirections(cmd->redirs))
			exit(1);
		exec_external_child(cmd, shell);
	}
	while (waitpid(pid, &status, 0) == -1)
	{
		if (errno != EINTR)
		{
			perror("minishell: waitpid");
			return (1);
		}
	}
	return (normalize_status(status));
}
