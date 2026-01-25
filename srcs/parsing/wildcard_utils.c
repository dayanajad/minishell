/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:10:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 18:44:40 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	match(const char *pattern, const char *text)
{
	if (!*pattern && !*text)
		return (true);
	if (*pattern == '*')
	{
		if (match(pattern + 1, text))
			return (true);
		if (*text && match(pattern, text + 1))
			return (true);
		return (false);
	}
	if (*pattern == *text)
		return (match(pattern + 1, text + 1));
	return (false);
}

static void	sort_matches(char **matches)
{
	int					i;
	int					j;
	char				*tmp;

	if (!matches)
		return ;
	i = 0;
	while (matches[i])
	{
		j = i + 1;
		while (matches[j])
		{
			if (ft_strcmp(matches[i], matches[j]) > 0)
			{
				tmp = matches[i];
				matches[i] = matches[j];
				matches[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	collect_matches(DIR *dir, const char *pattern, char ***res)
{
	struct dirent	*entry;
	char			*tmp;

	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] == '.' && pattern[0] != '.')
		{
			entry = readdir(dir);
			continue ;
		}
		if (match(pattern, entry->d_name))
		{
			tmp = ft_strdup(entry->d_name);
			av_push(res, tmp);
			free(tmp);
		}
		entry = readdir(dir);
	}
}

char	**get_matches(const char *pattern)
{
	DIR				*dir;
	char			**res;

	dir = opendir(".");
	if (!dir)
		return (NULL);
	res = NULL;
	collect_matches(dir, pattern, &res);
	closedir(dir);
	sort_matches(res);
	return (res);
}

void	insert_matches(t_tok **cur, char **matches)
{
	t_tok	*next;
	t_tok	*new;
	int		i;

	next = (*cur)->next;
	free((*cur)->value);
	(*cur)->value = ft_strdup(matches[0]);
	(*cur)->has_wildcard = false;
	i = 1;
	while (matches[i])
	{
		new = new_tok(TOK_WORD, ft_strdup(matches[i]), 0);
		new->next = next;
		(*cur)->next = new;
		*cur = new;
		i++;
	}
}
