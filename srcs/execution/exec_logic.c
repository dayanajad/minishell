/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_logic.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 07:20:57 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/12/11 07:20:58 by dbinti-m         ###   ########.fr       */
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

int	exec_and_node(t_ast *node, t_shell *shell)
{
	int	status;

	if (!node || !shell)
		return (1);
	status = exec_ast(node->u_data.branch.left, shell);
	if (status == 0)
		status = exec_ast(node->u_data.branch.right, shell);
	return (status);
}

int	exec_or_node(t_ast *node, t_shell *shell)
{
	int	status;

	if (!node || !shell)
		return (1);
	status = exec_ast(node->u_data.branch.left, shell);
	if (status != 0)
		status = exec_ast(node->u_data.branch.right, shell);
	return (status);
}

int	exec_subshell_node(t_ast *node, t_shell *shell)
{
	pid_t	pid;
	int		status;

	if (!node || !shell)
		return (1);
	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
	{
		set_child_signals();
		exit(exec_ast(node->u_data.child, shell));
	}
	waitpid(pid, &status, 0);
	return (normalize_status(status));
}
