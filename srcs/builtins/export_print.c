/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:00:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/26 12:00:00 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_escaped_len(const char *s)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '\\' || s[i] == '"' || s[i] == '$')
			len++;
		len++;
		i++;
	}
	return (len);
}

static char	*escape_export_value(const char *s)
{
	char	*out;
	int		i;
	int		j;

	if (!s)
		return (NULL);
	out = ft_calloc(count_escaped_len(s) + 1, sizeof(char));
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

static void	sort_env_arr(t_env **arr, int count)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = 0;
	while (i < count - 1)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(arr[i]->key, arr[j]->key) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	print_export(t_env *env)
{
	t_env	**arr;
	int		count;
	int		i;

	count = export_env_count(env);
	arr = ft_calloc(count + 1, sizeof(t_env *));
	if (!arr)
		return ;
	i = 0;
	while (env)
	{
		arr[i++] = env;
		env = env->next;
	}
	sort_env_arr(arr, count);
	i = 0;
	while (i < count)
	{
		if (ft_strcmp(arr[i]->key, "_") != 0)
			print_export_one(arr[i]);
		i++;
	}
	free(arr);
}
