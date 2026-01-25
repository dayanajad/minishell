/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:10:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:05:57 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	echo_suppresses_newline(char **av)
{
	int	i;
	int	j;

	if (!av)
		return (false);
	i = 1;
	while (av[i])
	{
		if (av[i][0] != '-' || av[i][1] != 'n')
			break ;
		j = 2;
		while (av[i][j] == 'n')
			j++;
		if (av[i][j] != '\0')
			break ;
		return (true);
	}
	return (false);
}

int	exec_builtin_echo(t_cmd *cmd, t_shell *shell)
{
	int	status;

	status = builtin_echo(cmd->av);
	if (shell && isatty(STDIN_FILENO) && echo_suppresses_newline(cmd->av))
		shell->noninteractive_prompt_newline = 1;
	return (status);
}
