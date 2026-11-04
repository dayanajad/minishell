/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:00:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:05:57 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_pipe_count(t_ast *ast)
{
	if (ast->type != A_PIPE)
		return (1);
	return (exec_pipe_count(ast->u_data.branch.left)
		+ exec_pipe_count(ast->u_data.branch.right));
}

void	exec_pipe_collect(t_ast *ast, t_ast **stages, int *index)
{
	if (ast->type == A_PIPE)
	{
		exec_pipe_collect(ast->u_data.branch.left, stages, index);
		exec_pipe_collect(ast->u_data.branch.right, stages, index);
	}
	else
		stages[(*index)++] = ast;
}

void	exec_pipe_child(t_ast *ast, t_shell *shell, int output,
		int pipefd[2])
{
	setup_signals_exec();
	if (output >= 0 && !dup_and_close(output, STDOUT_FILENO))
		exit(1);
	if (pipefd[0] >= 0)
	{
		close(pipefd[1]);
		if (!dup_and_close(pipefd[0], STDIN_FILENO))
			exit(1);
	}
	shell->in_child = PIPE_STAGE_CHILD;
	if (ast->type == A_SUBSHELL)
	{
		shell->in_child = 1;
		ast = ast->u_data.child;
	}
	exit(exec_ast(ast, shell));
}

bool	exec_pipe_cmd_has_input(t_ast *ast)
{
	t_redir	*redirs;

	if (!ast || ast->type != A_CMD || !ast->u_data.cmd)
		return (false);
	redirs = ast->u_data.cmd->redirs;
	while (redirs)
	{
		if (redirs->type == R_IN || redirs->type == R_HEREDOC)
			return (true);
		redirs = redirs->next;
	}
	return (false);
}

bool	exec_pipe_is_builtin_cmd(t_ast *ast)
{
	if (!ast || ast->type != A_CMD || !ast->u_data.cmd
		|| !ast->u_data.cmd->av || !ast->u_data.cmd->av[0])
		return (false);
	return (is_builtin(ast->u_data.cmd->av[0]));
}
