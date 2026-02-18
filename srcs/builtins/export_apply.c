/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_apply.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:00:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/26 12:00:00 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_invalid_id(char *arg, char *key)
{
	int	ret;

	ret = 1;
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	if (key && key[0] == '-' && key[1] != '\0')
		ret = 2;
	return (ret);
}

int	export_apply(char *arg, char *key, char *value, t_env **env)
{
	int	ret;

	ret = 0;
	if (!export_is_valid_identifier(key))
		ret = print_invalid_id(arg, key);
	else if (value)
		export_set_env_var(env, key, value);
	else if (!export_find_env_node(*env, key))
		export_set_env_var(env, key, NULL);
	free(key);
	free(value);
	return (ret);
}

static int	do_append(char *key, char *value, t_env **env)
{
	t_env	*node;
	char	*joined;

	node = export_find_env_node(*env, key);
	if (node)
	{
		joined = ft_strjoin(node->value, value);
		if (!joined)
		{
			free(key);
			free(value);
			return (1);
		}
		export_set_env_var(env, key, joined);
		free(joined);
	}
	else
		export_set_env_var(env, key, value);
	free(key);
	free(value);
	return (0);
}

int	export_apply_append(char *arg, char *key, char *value, t_env **env)
{
	int	ret;

	ret = 0;
	if (!export_is_valid_identifier(key))
	{
		ret = print_invalid_id(arg, key);
		free(key);
		free(value);
		return (ret);
	}
	return (do_append(key, value, env));
}
