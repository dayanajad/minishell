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

static int	handle_redir_only(t_cmd *cmd, t_shell *shell)
{
	int	saved[3];
	int	ret;

	if (save_stdio(saved) < 0)
		return (1);
	if (apply_redirections(cmd->redirs, shell))
		ret = 0;
	else
		ret = 1;
	restore_stdio(saved);
	return (ret);
}

static int	exec_builtin_with_redir(t_cmd *cmd, t_shell *shell)
{
	int	saved[3];
	int	ret;

	if (shell && shell->in_child)
	{
		if (!apply_redirections(cmd->redirs, shell))
			return (1);
		return (exec_builtin(cmd, shell));
	}
	if (save_stdio(saved) < 0)
		return (1);
	if (!apply_redirections(cmd->redirs, shell))
	{
		restore_stdio(saved);
		return (1);
	}
	ret = exec_builtin(cmd, shell);
	restore_stdio(saved);
	return (ret);
}

static int	handle_bare_cmd(t_cmd *cmd, t_shell *shell)
{
	char	*eq;

	if (ft_strcmp(cmd->av[0], "time") == 0 && !cmd->av[1])
	{
		ft_putstr_fd("real\t0m0.000s\n", STDERR_FILENO);
		ft_putstr_fd("user\t0m0.000s\n", STDERR_FILENO);
		ft_putstr_fd("sys\t0m0.000s\n", STDERR_FILENO);
		return (0);
	}
	eq = ft_strchr(cmd->av[0], '=');
	if (eq && cmd->av[0][0] != '=' && !cmd->av[1])
	{
		builtin_export((char *[]){"export", cmd->av[0], NULL}, &shell->env);
		return (0);
	}
	return (-1);
}

int	exec_cmd_node(t_ast *ast, t_shell *shell)
{
	t_cmd	*cmd;
	int		ret;

	cmd = ast->u_data.cmd;
	if (!cmd)
		return (0);
	if (!cmd->av || !cmd->av[0])
		return (handle_redir_only(cmd, shell));
	ret = handle_bare_cmd(cmd, shell);
	if (ret >= 0)
		return (ret);
	if (ft_strcmp(cmd->av[0], "env") == 0 && cmd->av[1])
		return (exec_external_cmd(cmd, shell));
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
	else if (ast->type == A_SEQ)
		status = exec_seq_node(ast, shell);
	else if (ast->type == A_SUBSHELL)
		status = exec_subshell_node(ast, shell);
	else if (ast->type == A_BG)
		status = exec_bg_node(ast, shell);
	shell->last_status = status;
	return (status);
}
