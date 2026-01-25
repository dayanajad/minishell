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

char	*resolve_cmd_path(t_cmd *cmd, t_shell *shell);

static void	update_underscore_var(t_shell *shell, t_cmd *cmd)
{
	int	last;

	if (!shell || !cmd || !cmd->av || !cmd->av[0])
		return ;
	last = 0;
	while (cmd->av[last + 1])
		last++;
	export_set_env_var(&shell->env, "_", cmd->av[last]);
}

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
	char		*path;
	char		**envp;
	struct stat	st;

	path = resolve_cmd_path(cmd, shell);
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		exec_print_is_dir(shell, path);
		free(path);
		exit(126);
	}
	envp = env_to_arr(shell->env);
	if (!envp)
	{
		free(path);
		exit(1);
	}
	execve(path, cmd->av, envp);
	if (errno == ENOEXEC && access(path, X_OK) == 0)
		exec_as_script(path, cmd, envp);
	exec_error_exit(shell, path);
}

int	exec_external_cmd(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;

	update_underscore_var(shell, cmd);
	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
	{
		setup_signals_exec();
		if (!apply_redirections(cmd->redirs, shell))
			exit(1);
		exec_external_child(cmd, shell);
	}
	return (exec_wait_for_child(pid));
}
