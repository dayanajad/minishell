/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 05:42:01 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/12/11 05:42:03 by dbinti-m         ###   ########.fr       */
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

static int	spawn_left(t_ast *ast, t_shell *shell, int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		set_child_signals();
		close(pipefd[0]);
		if (dup2(pipefd[1], STDOUT_FILENO) < 0)
			exit(1);
		close(pipefd[1]);
		exit(exec_ast(ast->u_data.branch.left, shell));
	}
	return (pid);
}

static int	spawn_right(t_ast *ast, t_shell *shell, int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		set_child_signals();
		close(pipefd[1]);
		if (dup2(pipefd[0], STDIN_FILENO) < 0)
			exit(1);
		close(pipefd[0]);
		exit(exec_ast(ast->u_data.branch.right, shell));
	}
	return (pid);
}

int	exec_pipe_node(t_ast *ast, t_shell *shell)
{
	int		pipefd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		left_status;
	int		right_status;

	if (pipe(pipefd) < 0)
	{
		perror("minishell: pipe");
		return (1);
	}
	left_pid = spawn_left(ast, shell, pipefd);
	right_pid = spawn_right(ast, shell, pipefd);
	close(pipefd[0]);
	close(pipefd[1]);
	if (left_pid > 0)
		waitpid(left_pid, &left_status, 0);
	else
		left_status = 0;
	if (right_pid > 0)
		waitpid(right_pid, &right_status, 0);
	else
		right_status = 0;
	(void)left_status;
	return (normalize_status(right_status));
}
