/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bg_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:25:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:05:57 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_bg_child(t_ast *ast, t_shell *shell, int outpipe[2], bool capture)
{
	shell->in_child = 1;
	if (capture)
	{
		close(outpipe[0]);
		dup2(outpipe[1], STDOUT_FILENO);
		close(outpipe[1]);
	}
	exec_ast(ast->u_data.branch.left, shell);
	exit(shell->last_status);
}

int	exec_bg_parent(t_ast *ast, t_shell *shell, int outpipe[2], pid_t pid)
{
	int	status;
	int	bg_status;

	if (!isatty(STDIN_FILENO))
	{
		close(outpipe[1]);
		status = exec_ast(ast->u_data.branch.right, shell);
		exec_bg_flush_output(outpipe[0]);
		close(outpipe[0]);
		waitpid(pid, &bg_status, 0);
		return (status);
	}
	ft_putstr_fd("[1] ", 1);
	ft_putnbr_fd(pid, 1);
	ft_putstr_fd("\n", 1);
	if (ast->u_data.branch.right)
		return (exec_ast(ast->u_data.branch.right, shell));
	return (0);
}
