/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 19:42:21 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/12/23 23:59:18 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_builtin(const char *name)
{
	if (!name)
		return (false);
	if (ft_strcmp(name, "echo"))
		return (true);
	if (ft_strcmp(name, "cd"))
		return (true);
	if (ft_strcmp(name, "pwd"))
		return (true);
	if (ft_strcmp(name, "export"))
		return (true);
	if (ft_strcmp(name, "unset"))
		return (true);
	if (ft_strcmp(name, "env"))
		return (true);
	if (ft_strcmp(name, "exit"))
		return (true);
	return (false);
}

int	exec_builtin(t_cmd *cmd, t_shell *shell)
{
	char	*name;

	if (!cmd || !cmd->av || !cmd->av[0])
		return (1);
	name = cmd->av[0];
	if (ft_strcmp(name, "echo") == 0)
		return (builtin_echo(cmd->av));
	if (ft_strcmp(name, "cd") == 0)
		return (builtin_cd(cmd->av, shell));
	if (ft_strcmp(name, "pwd") == 0)
		return (builtin_pwd());
	if (ft_strcmp(name, "env") == 0)
		return (builtin_env(shell->env));
	if (ft_strcmp(name, "export") == 0)
		return (builtin_export(cmd->av, &shell->env));
	if (ft_strcmp(name, "unset") == 0)
		return (builtin_unset(cmd->av, &shell->env));
	if (ft_strcmp(name, "exit") == 0)
		return (builtin_exit(cmd->av, shell));
	return (1);
}
