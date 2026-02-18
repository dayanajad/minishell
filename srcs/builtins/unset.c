/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 17:34:16 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/12 17:39:15 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	unset_invalid_option(char opt)
{
	ft_putstr_fd("minishell: unset: -", STDERR_FILENO);
	ft_putchar_fd(opt, STDERR_FILENO);
	ft_putendl_fd(": invalid option", STDERR_FILENO);
	ft_putendl_fd("unset: usage: unset [-f] [-v] [-n] [name ...]",
		STDERR_FILENO);
	return (2);
}

static void	remove_env_node(t_env **env, const char *key)
{
	t_env	*cur;
	t_env	*prev;

	cur = *env;
	prev = NULL;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			if (prev == NULL)
				*env = cur->next;
			else
				prev->next = cur->next;
			free(cur->key);
			free(cur->value);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}

int	builtin_unset(char **av, t_env **env)
{
	int		i;
	bool	no_flags;

	i = 1;
	no_flags = false;
	while (av[i])
	{
		if (!no_flags && av[i][0] == '-' && av[i][1] != '\0')
		{
			if (ft_strcmp(av[i], "--") == 0)
			{
				no_flags = true;
				i++;
				continue ;
			}
			return (unset_invalid_option(av[i][1]));
		}
		remove_env_node(env, av[i]);
		i++;
	}
	return (0);
}
