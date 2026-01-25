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

int		export_apply(char *arg, char *key, char *value, t_env **env);
int		export_apply_append(char *arg, char *key, char *val, t_env **env);
void	print_export(t_env *env);

static void	parse_export_arg(char *arg, char **key, char **value, bool *append)
{
	char	*equal;

	equal = ft_strchr(arg, '=');
	*append = false;
	if (equal)
	{
		if (equal > arg && *(equal - 1) == '+')
		{
			*append = true;
			*key = ft_substr(arg, 0, (equal - 1) - arg);
		}
		else
			*key = ft_substr(arg, 0, equal - arg);
		*value = ft_strdup(equal + 1);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
}

static int	export_one(char *arg, t_env **env)
{
	char	*key;
	char	*value;
	bool	append;

	parse_export_arg(arg, &key, &value, &append);
	if (!key)
		return (1);
	if (append)
		return (export_apply_append(arg, key, value, env));
	return (export_apply(arg, key, value, env));
}

int	builtin_export(char **av, t_env **env)
{
	int	i;
	int	ret;
	int	rv;

	if (av[1] == NULL)
	{
		print_export(*env);
		return (0);
	}
	ret = 0;
	i = 1;
	while (av[i])
	{
		rv = export_one(av[i], env);
		if (rv > ret)
			ret = rv;
		i++;
	}
	return (ret);
}
