/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 05:26:17 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/12/11 05:26:19 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_identifier(const char *s)
{
	size_t	i;

	if (!s || !*s)
		return (false);
	if (!(ft_isalpha((unsigned char)s[0]) || s[0] == '_'))
		return (false);
	i = 1;
	while (s[i])
	{
		if (!(ft_isalnum((unsigned char)s[i]) || s[i] == '_'))
			return (false);
		i++;
	}
	return (true);
}

static void	unset_one(t_env **env, const char *key)
{
	t_env	*cur;
	t_env	*prev;

	prev = NULL;
	cur = *env;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			if (prev)
				prev->next = cur->next;
			else
				*env = cur->next;
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
	int	i;
	int	status;

	status = 0;
	i = 1;
	while (av[i])
	{
		if (!is_valid_identifier(av[i]))
		{
			ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
			ft_putstr_fd(av[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			status = 1;
		}
		else
			unset_one(env, av[i]);
		i++;
	}
	return (status);
}
