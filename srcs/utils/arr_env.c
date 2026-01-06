/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_arr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/28 11:24:33 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/12/28 12:07:10 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*new_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	node->value = ft_strdup(value);
	if (!node->key || !node->value)
	{
		free(node->key);
		free(node->value);
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

static void	add_env_back(t_env **head, t_env *new)
{
	t_env	*cur;

	if (!*head)
	{
		*head = new;
		return ;
	}
	cur = *head;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

static t_env	*parse_env_str(char *str)
{
	char	*equals;
	char	*key;
	char	*value;
	t_env	*node;
	int		len;

	equals = ft_strchr(str, '=');
	if (!equals)
		return (new_env_node(str, ""));
	len = equals - str;
	key = malloc(len + 1);
	if (!key)
		return (NULL);
	ft_strlcpy(key, str, len + 1);
	value = ft_strdup(equals + 1);
	if (!value)
	{
		free(key);
		return (NULL);
	}
	node = new_env_node(key, value);
	free(key);
	free(value);
	return (node);
}

t_env	*arr_to_env(char **envp)
{
	t_env	*head;
	t_env	*node;
	int		i;

	if (!envp)
		return (NULL);
	head = NULL;
	i = 0;
	while (envp[i])
	{
		node = parse_env_str(envp[i]);
		if (!node)
		{
			free_env(head);
			return (NULL);
		}
		add_env_back(&head, node);
		i++;
	}
	return (head);
}
