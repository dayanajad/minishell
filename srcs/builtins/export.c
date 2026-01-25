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

static int	cmp_keys(const void *a, const void *b)
{
	const t_env	*ea;
	const t_env	*eb;

	ea = *(const t_env **)a;
	eb = *(const t_env **)b;
	if (!ea || !ea->key)
		return (-1);
	if (!eb || !eb->key)
		return (1);
	return (ft_strcmp(ea->key, eb->key));
}

static int	env_count(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

static char	*escape_export_value(const char *s)
{
	char	*out;
	int	len;
	int	i;
	int	j;

	if (!s)
		return (NULL);
	len = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '\\' || s[i] == '"' || s[i] == '$')
			len++;
		len++;
		i++;
	}
	out = ft_calloc(len + 1, sizeof(char));
	if (!out)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] == '\\' || s[i] == '"' || s[i] == '$')
			out[j++] = '\\';
		out[j++] = s[i++];
	}
	out[j] = '\0';
	return (out);
}

static void	print_export_one(const t_env *env)
{
	char	*escaped;

	if (!env || !env->key)
		return ;
	ft_putstr_fd("declare -x ", 1);
	ft_putstr_fd(env->key, 1);
	if (env->value)
	{
		ft_putstr_fd("=\"", 1);
		escaped = escape_export_value(env->value);
		if (escaped)
		{
			ft_putstr_fd(escaped, 1);
			free(escaped);
		}
		else
			ft_putstr_fd(env->value, 1);
		ft_putstr_fd("\"", 1);
	}
	ft_putstr_fd("\n", 1);
}

static void	print_export(t_env *env)
{
	t_env	**arr;
	int	count;
	int	i;

	count = env_count(env);
	arr = ft_calloc(count + 1, sizeof(t_env *));
	if (!arr)
		return ;
	i = 0;
	while (env)
	{
		arr[i++] = env;
		env = env->next;
	}
	qsort(arr, count, sizeof(t_env *), cmp_keys);
	i = 0;
	while (i < count)
	{
		if (ft_strcmp(arr[i]->key, "_") != 0)
			print_export_one(arr[i]);
		i++;
	}
	free(arr);
}

static int	export_apply(char *arg, char *key, char *value, t_env **env)
{
	int	ret;

	ret = 0;
	if (!export_is_valid_identifier(key))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		ret = 1;
		if (key && key[0] == '-' && key[1] != '\0')
			ret = 2;
	}
	else if (value)
		export_set_env_var(env, key, value);
	free(key);
	free(value);
	return (ret);
}

static int	export_apply_append(char *arg, char *key, char *value, t_env **env)
{
	t_env	*node;
	char	*joined;
	int	ret;

	ret = 0;
	if (!export_is_valid_identifier(key))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		ret = 1;
		if (key && key[0] == '-' && key[1] != '\0')
			ret = 2;
		free(key);
		free(value);
		return (ret);
	}
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

static int	export_one(char *arg, t_env **env)
{
	char	*equal;
	char	*key;
	char	*value;
	bool	append;

	equal = ft_strchr(arg, '=');
	append = false;
	if (equal)
	{
		if (equal > arg && *(equal - 1) == '+')
		{
			append = true;
			key = ft_substr(arg, 0, (equal - 1) - arg);
		}
		else
			key = ft_substr(arg, 0, equal - arg);
		value = ft_strdup(equal + 1);
	}
	else
	{
		key = ft_strdup(arg);
		value = NULL;
	}
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
