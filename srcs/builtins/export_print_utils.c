/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:00:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/26 12:00:00 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export_cmp_keys(const void *a, const void *b)
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

int	export_env_count(t_env *env)
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
