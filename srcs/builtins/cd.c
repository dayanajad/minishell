/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 05:24:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/12/11 05:24:01 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*env_find(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

static void	set_env_val(t_env **env, const char *key, const char *value)
{
	t_env	*node;

	node = env_find(*env, key);
	if (!node)
	{
		node = malloc(sizeof(*node));
		if (!node)
			return ;
		node->key = ft_strdup(key);
		node->value = NULL;
		node->next = *env;
		if (!node->key)
		{
			free(node);
			return ;
		}
		*env = node;
	}
	if (node->value)
		free(node->value);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
}

static int	cd_print_not_set(const char *name)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd((char *)name, STDERR_FILENO);
	ft_putstr_fd(" not set\n", STDERR_FILENO);
	return (1);
}

static int	cd_change_dir(t_shell *shell, const char *target)
{
	char	buf[PATH_MAX];
	char	*oldpwd;

	if (!getcwd(buf, sizeof(buf)))
		oldpwd = NULL;
	else
		oldpwd = ft_strdup(buf);
	if (chdir(target) < 0)
	{
		free(oldpwd);
		perror("minishell: cd");
		return (1);
	}
	if (oldpwd)
	{
		set_env_val(&shell->env, "OLDPWD", oldpwd);
		free(oldpwd);
	}
	if (getcwd(buf, sizeof(buf)))
		set_env_val(&shell->env, "PWD", buf);
	return (0);
}

int	builtin_cd(char **av, t_shell *shell)
{
	char	*target;

	if (av[1] && av[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	if (!av[1])
	{
		target = get_env_value(shell->env, "HOME");
		if (!target)
			return (cd_print_not_set("HOME"));
	}
	else if (ft_strcmp(av[1], "-") == 0)
	{
		target = get_env_value(shell->env, "OLDPWD");
		if (!target)
			return (cd_print_not_set("OLDPWD"));
	}
	else
		target = av[1];
	return (cd_change_dir(shell, target));
}
