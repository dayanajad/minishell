/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_logic.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 18:20:41 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/12 18:23:53 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_and_node(t_ast *ast, t_shell *shell)
{
	int	status;

	status = exec_ast(ast->u_data.branch.left, shell);
	if (status == 0)
		status = exec_ast(ast->u_data.branch.right, shell);
	return (status);
}

int	exec_or_node(t_ast *ast, t_shell *shell)
{
	int	status;

	status = exec_ast(ast->u_data.branch.left, shell);
	if (status != 0)
		status = exec_ast(ast->u_data.branch.right, shell);
	return (status);
}

int	exec_seq_node(t_ast *ast, t_shell *shell)
{
	exec_ast(ast->u_data.branch.left, shell);
	return (exec_ast(ast->u_data.branch.right, shell));
}
