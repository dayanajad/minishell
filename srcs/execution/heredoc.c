/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 05:39:06 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/12/11 05:39:08 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	process_cmd_heredocs(t_cmd *cmd, int *idx, t_shell *shell)
{
	t_redir	*r;

	if (!cmd)
		return (true);
	r = cmd->redirs;
	while (r)
	{
		if (r->type == R_HEREDOC)
		{
			if (!heredoc_create_file(r, *idx, shell))
				return (false);
			(*idx)++;
		}
		r = r->next;
	}
	return (true);
}

static bool	process_heredocs_rec(t_ast *ast, int *idx, t_shell *shell)
{
	if (!ast)
		return (true);
	if (ast->type == A_CMD)
		return (process_cmd_heredocs(ast->u_data.cmd, idx, shell));
	if (ast->type == A_PIPE || ast->type == A_AND || ast->type == A_OR)
	{
		if (!process_heredocs_rec(ast->u_data.branch.left, idx, shell))
			return (false);
		return (process_heredocs_rec(ast->u_data.branch.right, idx, shell));
	}
	if (ast->type == A_SUBSHELL)
		return (process_heredocs_rec(ast->u_data.child, idx, shell));
	return (true);
}

bool	process_heredocs(t_ast *ast, t_shell *shell)
{
	int		index;
	bool	ok;

	if (!ast || !shell)
		return (true);
	index = 0;
	g_signal = 0;
	set_heredoc_signals();
	ok = process_heredocs_rec(ast, &index, shell);
	set_parent_signals();
	if (!ok)
		return (false);
	if (g_signal == SIGINT)
		return (false);
	return (true);
}
