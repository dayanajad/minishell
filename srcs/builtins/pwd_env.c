/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 23:38:57 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/07 00:41:45 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		perror("minishell: pwd");
		return (1);
	}
	ft_putstr_fd(cwd, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

int	builtin_env(t_env *env)
{
	t_env	*cur;

	cur = env;
	while (cur)
	{
		if (cur->value)
		{
			ft_putstr_fd(cur->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putstr_fd(cur->value, STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		cur = cur->next;
	}
	return (0);
}
