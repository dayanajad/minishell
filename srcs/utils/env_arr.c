/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_arr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/28 12:12:19 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/12/28 12:27:25 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_env_nodes(t_env *env)
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

static char	*create_env_str(t_env *node)
{
	char	*str;
	int		len;

	len = ft_strlen(node->key) + ft_strlen(node->value) + 2;
	str = malloc(len);
	if (!str)
		return (NULL);
	ft_strlcpy(str, node->key, len);
	ft_strlcat(str, "=", len);
	ft_strlcat(str, node->value, len);
	return (str);
}

char	**env_to_arr(t_env *env)
{
	char	**arr;
	int		count;
	int		i;

	count = count_env_nodes(env);
	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (env)
	{
		arr[i] = create_env_str(env);
		if (!arr[i])
		{
			free_str_arr(arr);
			return (NULL);
		}
		env = env->next;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}
