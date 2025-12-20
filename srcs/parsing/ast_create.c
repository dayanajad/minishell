/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_create.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 18:34:25 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/12/08 18:34:28 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
	{
		perror("minishell: malloc");
		exit(EXIT_FAILURE);
	}
	cmd->av = NULL;
	cmd->redirs = NULL;
	return (cmd);
}

t_ast	*new_ast_cmd(t_cmd *cmd)
{
	t_ast	*node;

	node = (t_ast *)malloc(sizeof(t_ast));
	if (!node)
	{
		perror("minishell: malloc");
		exit(EXIT_FAILURE);
	}
	node->type = A_CMD;
	node->u_data.cmd = cmd;
	return (node);
}

t_ast	*new_ast_bin(t_ast_type type, t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = (t_ast *)malloc(sizeof(t_ast));
	if (!node)
	{
		perror("minishell: malloc");
		exit(EXIT_FAILURE);
	}
	node->type = type;
	node->u_data.branch.left = left;
	node->u_data.branch.right = right;
	return (node);
}

t_ast	*new_ast_subshell(t_ast *child)
{
	t_ast	*node;

	node = (t_ast *)malloc(sizeof(t_ast));
	if (!node)
	{
		perror("minishell: malloc");
		exit(EXIT_FAILURE);
	}
	node->type = A_SUBSHELL;
	node->u_data.child = child;
	return (node);
}

t_redir	*new_redir(t_redir_type type, const char *target)
{
	t_redir	*r;

	r = (t_redir *)malloc(sizeof(t_redir));
	if (!r)
	{
		perror("minishell: malloc");
		exit(EXIT_FAILURE);
	}
	r->type = type;
	r->target = ft_strdup(target);
	if (!r->target)
	{
		perror("minishell: strdup");
		exit(EXIT_FAILURE);
	}
	r->next = NULL;
	return (r);
}
