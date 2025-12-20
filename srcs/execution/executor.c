/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 05:43:19 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/12/11 05:43:21 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	normalize_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

static void	exec_external_child(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	char	**envp;

	set_child_signals();
	path = find_in_path(cmd->av[0], shell->env);
	if (!path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->av[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit(127);
	}
	envp = env_to_array(shell->env);
	if (!envp)
	{
		perror("minishell");
		free(path);
		exit(126);
	}
	execve(path, cmd->av, envp);
	perror("minishell: execve");
	free(path);
	free_str_arr(envp);
	exit(126);
}

static int	exec_cmd_node(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	if (!cmd)
		return (0);
	if (!apply_redirections(cmd->redirs))
		return (1);
	if (!cmd->av || !cmd->av[0])
		return (0);
	if (is_builtin(cmd->av[0]))
		return (exec_builtin(cmd, shell));
	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
		exec_external_child(cmd, shell);
	waitpid(pid, &status, 0);
	return (normalize_status(status));
}

static int	exec_node(t_ast *node, t_shell *shell)
{
	if (!node)
		return (0);
	if (node->type == A_CMD)
		return (exec_cmd_node(node->u_data.cmd, shell));
	if (node->type == A_PIPE)
		return (exec_pipe_node(node, shell));
	if (node->type == A_AND)
		return (exec_and_node(node, shell));
	if (node->type == A_OR)
		return (exec_or_node(node, shell));
	if (node->type == A_SUBSHELL)
		return (exec_subshell_node(node, shell));
	return (0);
}

int	exec_ast(t_ast *ast, t_shell *shell)
{
	int	status;

	if (!ast || !shell)
		return (1);
	status = exec_node(ast, shell);
	shell->last_status = status;
	return (status);
}
