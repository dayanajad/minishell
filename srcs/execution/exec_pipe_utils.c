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
