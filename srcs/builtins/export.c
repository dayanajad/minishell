/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 17:51:52 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/12 18:06:02 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_export(t_env *env)
{
	while (env)
	{
		printf("declare -x %s=\"%s\"\n", env->key, env->value);
		env = env->next;
	}
}

static t_env	*find_env_node(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

static t_env	*create_env_node(const char *key, const char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	if (!new->key || !new->value)
	{
		free(new->key);
		free(new->value);
		free(new);
		return (NULL);
	}
	new->next = NULL;
	return (new);
}

static void	set_env_var(t_env **env, char *key, char *value)
{
	t_env	*node;
	t_env	*new;

	node = find_env_node(*env, key);
	if (node)
	{
		free(node->value);
		node->value = ft_strdup(value);
	}
	else
	{
		new = create_env_node(key, value);
		if (new)
		{
			new->next = *env;
			*env = new;
		}
	}
}

int	builtin_export(char **av, t_env **env)
{
	int		i;
	char	*equal;
	char	*key;
	char	*value;

	if (av[1] == NULL)
	{
		print_export(*env);
		return (0);
	}
	i = 1;
	while (av[i])
	{
		equal = ft_strchr(av[i], '=');
		if (equal)
		{
			key = ft_substr(av[i], 0, equal - av[i]);
			value = ft_strdup(equal + 1);
			set_env_var(env, key, value);
			free(key);
			free(value);
		}
		i++;
	}
	return (0);
}
