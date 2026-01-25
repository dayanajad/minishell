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
	int	devnull;

	shell->in_child = 1;
	setup_signals_exec();
	if (!isatty(STDIN_FILENO) && !exec_pipe_cmd_has_input(ast))
	{
		devnull = open("/dev/null", O_RDONLY);
		if (devnull >= 0)
		{
			dup2(devnull, STDIN_FILENO);
			close(devnull);
		}
	}
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	exit(exec_ast(ast, shell));
}

static void	exec_pipe_child_right(t_ast *ast, t_shell *shell, int pipefd[2],
		t_pipe_flags flags)
{
	shell->in_child = 1;
	setup_signals_exec();
	close(pipefd[1]);
	if (flags.use_pipe)
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
	}
	if (!flags.use_pipe && !flags.keep_pipe_open)
		close(pipefd[0]);
	exit(exec_ast(ast, shell));
}

static int	wait_pipe_children(t_pipe_state *st, bool left_is_builtin)
{
	waitpid(st->pid_left, &st->status_left, 0);
	(void)left_is_builtin;
	waitpid(st->pid_right, &st->status, 0);
	return (normalize_status(st->status));
}

int	exec_pipe_node(t_ast *ast, t_shell *shell)
{
	t_pipe_state	st;
	t_pipe_flags	flags;
	bool			left_is_builtin;

	if (pipe(st.pipefd) < 0)
	{
		perror("minishell: pipe");
		return (1);
	}
	st.right_has_stdin_redir = exec_pipe_cmd_has_input(
			ast->u_data.branch.right);
	st.right_is_builtin = exec_pipe_is_builtin_cmd(ast->u_data.branch.right);
	left_is_builtin = exec_pipe_is_builtin_cmd(ast->u_data.branch.left);
	flags.use_pipe = !st.right_has_stdin_redir;
	flags.keep_pipe_open = (st.right_has_stdin_redir && !st.right_is_builtin);
	st.pid_right = fork();
	if (st.pid_right == 0)
		exec_pipe_child_right(ast->u_data.branch.right, shell, st.pipefd,
			flags);
	close(st.pipefd[0]);
	st.pid_left = fork();
	if (st.pid_left == 0)
		exec_pipe_child_left(ast->u_data.branch.left, shell, st.pipefd);
	close(st.pipefd[1]);
	return (wait_pipe_children(&st, left_is_builtin));
}
