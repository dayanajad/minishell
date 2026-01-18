/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 17:31:28 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/07 00:39:04 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_redir_only(t_cmd *cmd)
{
	int	saved[2];
	int	ret;

	if (save_stdio(saved) < 0)
		return (1);
	if (apply_redirections(cmd->redirs))
		ret = 0;
	else
		ret = 1;
	restore_stdio(saved);
	return (ret);
}

static int	exec_builtin_with_redir(t_cmd *cmd, t_shell *shell)
{
	int	saved[2];
	int	ret;

	if (save_stdio(saved) < 0)
		return (1);
	if (!apply_redirections(cmd->redirs))
	{
		restore_stdio(saved);
		return (1);
	}
	ret = exec_builtin(cmd, shell);
	restore_stdio(saved);
	return (ret);
}

int	exec_cmd_node(t_ast *ast, t_shell *shell)
{
	t_cmd	*cmd;

	cmd = ast->u_data.cmd;
	if (!cmd)
		return (0);
	if (!cmd->av || !cmd->av[0])
		return (handle_redir_only(cmd));
	if (is_builtin(cmd->av[0]))
		return (exec_builtin_with_redir(cmd, shell));
	return (exec_external_cmd(cmd, shell));
}

int	exec_ast(t_ast *ast, t_shell *shell)
{
	int	status;

	status = 0;
	if (!ast || !shell)
		return (0);
	if (ast->type == A_CMD)
		status = exec_cmd_node(ast, shell);
	else if (ast->type == A_PIPE)
		status = exec_pipe_node(ast, shell);
	else if (ast->type == A_AND)
		status = exec_and_node(ast, shell);
	else if (ast->type == A_OR)
		status = exec_or_node(ast, shell);
	else if (ast->type == A_SUBSHELL)
		status = exec_subshell_node(ast, shell);
	shell->last_status = status;
	return (status);
}
