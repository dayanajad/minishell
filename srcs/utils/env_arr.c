/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_arr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 07:44:47 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/12/11 07:44:51 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	fill_env_array(t_env *env, char **arr)
{
	int		i;
	char	*tmp;

	i = 0;
	while (env)
	{
		tmp = ft_strjoin(env->key, "=");
		if (!tmp)
			return (1);
		if (env->value)
			arr[i] = ft_strjoin(tmp, env->value);
		else
			arr[i] = ft_strdup(tmp);
		free(tmp);
		if (!arr[i])
			return (1);
		i++;
		env = env->next;
	}
	arr[i] = NULL;
	return (0);
}

char	**env_to_array(t_env *env)
{
	int		len;
	t_env	*cur;
	char	**arr;

	len = 0;
	cur = env;
	while (cur)
	{
		len++;
		cur = cur->next;
	}
	arr = malloc(sizeof(char *) * (len + 1));
	if (!arr)
		return (NULL);
	if (fill_env_array(env, arr))
	{
		free_str_arr(arr);
		return (NULL);
	}
	return (arr);
}
