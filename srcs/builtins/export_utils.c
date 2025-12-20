/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 06:15:10 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/12/11 06:15:11 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_identifier(const char *s)
{
	size_t	i;

	if (!s || !s[0])
		return (false);
	if (!(ft_isalpha((unsigned char)s[0]) || s[0] == '_'))
		return (false);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!(ft_isalnum((unsigned char)s[i]) || s[i] == '_'))
			return (false);
		i++;
	}
	return (true);
}

static t_env	*env_add_new_node(t_env **env, const char *key)
{
	t_env	*node;
	t_env	*cur;

	node = malloc(sizeof(*node));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (!node->key)
	{
		free(node);
		return (NULL);
	}
	node->value = NULL;
	node->next = NULL;
	if (!*env)
		*env = node;
	else
	{
		cur = *env;
		while (cur->next)
			cur = cur->next;
		cur->next = node;
	}
	return (node);
}

static void	env_set(t_env **env, const char *key, const char *value)
{
	t_env	*node;

	node = *env;
	while (node && ft_strcmp(node->key, key) != 0)
		node = node->next;
	if (!node)
		node = env_add_new_node(env, key);
	if (!node)
		return ;
	if (node->value)
		free(node->value);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
}

int	export_one_arg(char *arg, t_env **env)
{
	char	*eq;
	char	*key;
	char	*val;

	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		return (1);
	}
	eq = ft_strchr(arg, '=');
	if (!eq)
	{
		env_set(env, arg, NULL);
		return (0);
	}
	key = ft_substr(arg, 0, eq - arg);
	val = ft_strdup(eq + 1);
	if (key && val)
		env_set(env, key, val);
	free(key);
	free(val);
	return (0);
}
