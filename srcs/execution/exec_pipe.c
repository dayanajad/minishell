/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 20:27:38 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/12 21:09:59 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_pipe_child_left(t_ast *ast, t_shell *shell, int pipefd[2])
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	exit(exec_ast(ast, shell));
}

static void	exec_pipe_child_right(t_ast *ast, t_shell *shell, int pipefd[2])
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	exit(exec_ast(ast, shell));
}

int	exec_pipe_node(t_ast *ast, t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		status;

	if (pipe(pipefd) < 0)
	{
		perror("minishell: pipe");
		return (1);
	}
	pid_left = fork();
	if (pid_left == 0)
		exec_pipe_child_left(ast->u_data.branch.left, shell, pipefd);
	pid_right = fork();
	if (pid_right == 0)
		exec_pipe_child_right(ast->u_data.branch.right, shell, pipefd);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid_left, NULL, 0);
	waitpid(pid_right, &status, 0);
	return (normalize_status(status));
}
