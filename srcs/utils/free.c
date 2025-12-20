/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 23:24:52 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/12/07 23:27:43 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_str_arr(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

void	free_redirs(t_redir *redirs)
{
	t_redir	*next;

	while (redirs)
	{
		next = redirs->next;
		free(redirs->target);
		free(redirs);
		redirs = next;
	}
}

void	free_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->av)
	{
		i = 0;
		while (cmd->av[i])
		{
			free(cmd->av[i]);
			i++;
		}
		free(cmd->av);
	}
	free_redirs(cmd->redirs);
	free(cmd);
}

void	free_ast(t_ast *ast)
{
	if (!ast)
		return ;
	if (ast->type == A_CMD)
		free_cmd(ast->u_data.cmd);
	else if (ast->type == A_SUBSHELL)
		free_ast(ast->u_data.child);
	else if (ast->type == A_PIPE || ast->type == A_AND || ast->type == A_OR)
	{
		free_ast(ast->u_data.branch.left);
		free_ast(ast->u_data.branch.right);
	}
	free(ast);
}
