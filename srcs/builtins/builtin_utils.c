/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 04:40:02 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/12/11 04:40:05 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_builtin(const char *name)
{
	if (!name)
		return (false);
	if (ft_strcmp(name, "echo") == 0)
		return (true);
	if (ft_strcmp(name, "cd") == 0)
		return (true);
	if (ft_strcmp(name, "pwd") == 0)
		return (true);
	if (ft_strcmp(name, "export") == 0)
		return (true);
	if (ft_strcmp(name, "unset") == 0)
		return (true);
	if (ft_strcmp(name, "env") == 0)
		return (true);
	if (ft_strcmp(name, "exit") == 0)
		return (true);
	return (false);
}

int	exec_builtin(t_cmd *cmd, t_shell *shell)
{
	char	**av;

	if (!cmd || !cmd->av || !cmd->av[0])
		return (0);
	av = cmd->av;
	if (ft_strcmp(av[0], "echo") == 0)
		return (builtin_echo(av));
	if (ft_strcmp(av[0], "cd") == 0)
		return (builtin_cd(av, shell));
	if (ft_strcmp(av[0], "pwd") == 0)
		return (builtin_pwd());
	if (ft_strcmp(av[0], "export") == 0)
		return (builtin_export(av, &shell->env));
	if (ft_strcmp(av[0], "unset") == 0)
		return (builtin_unset(av, &shell->env));
	if (ft_strcmp(av[0], "env") == 0)
		return (builtin_env(shell->env));
	if (ft_strcmp(av[0], "exit") == 0)
		return (builtin_exit(av, shell));
	return (0);
}
