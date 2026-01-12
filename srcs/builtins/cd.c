/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 19:47:26 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/11 20:14:37 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_target_dir(char **av, t_env *env)
{
	char	*target;

	if (!av[1] || ft_strcmp(av[1], "~") == 0)
	{
		target = get_env_value(env, "HOME");
		if (!target)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			return (NULL);
		}
		return (target);
	}
	if (ft_strcmp(av[1], "-") == 0)
	{
		target = get_env_value(env, "OLDPWD");
		if (!target)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
			return (NULL);
		}
		ft_putstr_fd(target, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		return (target);
	}
	return (av[1]);
}

static void	update_env_var(t_env **env, char *key, char *value)
{
	t_env	*cur;
	t_env	*new_node;

	cur = *env;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			free(cur->value);
			cur->value = ft_strdup(value);
			return ;
		}
		cur = cur->next;
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = *env;
	*env = new_node;
}

static int	cd_error(char *target)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(target, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	perror("");
	return (1);
}

int	builtin_cd(char **av, t_shell *shell)
{
	char	*target;
	char	old_pwd[PATH_MAX];

	if (av[1] && av[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	target = get_target_dir(av, shell->env);
	if (!target)
		return (1);
	if (getcwd(old_pwd, PATH_MAX) == NULL)
		old_pwd[0] = '\0';
	if (chdir(target) != 0)
		return (cd_error(target));
	if (old_pwd[0])
		update_env_var(&shell->env, "OLDPWD", old_pwd);
	if (getcwd(old_pwd, PATH_MAX))
		update_env_var(&shell->env, "PWD", old_pwd);
	return (0);
}
