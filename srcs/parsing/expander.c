/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:54:10 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:05:57 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_tilde_tok(t_tok *tok, t_shell *shell)
{
	char	*home;
	char	*joined;

	if (!tok || tok->type != TOK_WORD || tok->was_quoted)
		return ;
	if (!tok->value || tok->value[0] != '~')
		return ;
	if (tok->value[1] && tok->value[1] != '/')
		return ;
	home = get_env_value(shell->env, "HOME");
	if (!home)
		return ;
	joined = ft_strjoin(home, tok->value + 1);
	if (!joined)
		return ;
	free(tok->value);
	tok->value = joined;
}

static char	*whitespace_to_space(const char *s)
{
	char	*tmp;
	int		i;

	if (!s)
		return (NULL);
	tmp = ft_strdup(s);
	if (!tmp)
		return (NULL);
	i = 0;
	while (tmp[i])
	{
		if (tmp[i] == '\t' || tmp[i] == '\n')
			tmp[i] = ' ';
		i++;
	}
	return (tmp);
}

static bool	has_ifs_space(const char *s)
{
	int	i;

	if (!s)
		return (false);
	i = 0;
	while (s[i])
	{
		if (s[i] == ' ' || s[i] == '\t' || s[i] == '\n')
			return (true);
		i++;
	}
	return (false);
}

static bool	word_has_wildcard(const char *s)
{
	if (!s)
		return (false);
	return (ft_strchr(s, '*') != NULL);
}

static t_tok	*perform_split_tok(t_tok **tokens, t_tok *prev, t_tok *cur)
{
	char	**parts;
	char	*tmp;
	t_tok	*next_tok;
	t_tok	*last;
	char	*dup;
	t_tok	*new_tok_node;
	int		i;
	bool	start_ws;
	bool	end_ws;
	bool	was_join;

	next_tok = cur->next;
	was_join = cur->join_next;
	start_ws = (cur->value[0] == ' ' || cur->value[0] == '\t' || cur->value[0] == '\n');
	if (ft_strlen(cur->value) > 0)
		end_ws = (cur->value[ft_strlen(cur->value) - 1] == ' ' || 
				  cur->value[ft_strlen(cur->value) - 1] == '\t' || 
				  cur->value[ft_strlen(cur->value) - 1] == '\n');
	else
		end_ws = false;

	if (start_ws && prev)
		prev->join_next = false;
	
	tmp = whitespace_to_space(cur->value);
	if (!tmp)
		return (cur);
	parts = ft_split(tmp, ' ');
	free(tmp);
	free(cur->value);
	free(cur);

	if (!parts || !parts[0])
	{
		// Should return prev if it exists.
		// Also free_str_arr helper expected to handle NULL or empty?
		// Usually ft_split returns NULL on malloc fail or pointer to array.
		if (parts)
			free_str_arr(parts);
		
		if (prev)
			prev->next = next_tok;
		else
			*tokens = next_tok;
		return (prev ? prev : NULL);
	}

	last = prev;
	i = 0;
	while (parts[i])
	{
		dup = ft_strdup(parts[i]);
		if (!dup)
			break ;
		new_tok_node = new_tok(TOK_WORD, dup, 0);
		if (!new_tok_node)
		{
			free(dup);
			break ;
		}
		new_tok_node->has_wildcard = word_has_wildcard(new_tok_node->value);
		new_tok_node->was_quoted = false;
		
		new_tok_node->join_next = false; 
		
		if (last)
			last->next = new_tok_node;
		else
			*tokens = new_tok_node;
		last = new_tok_node;
		i++;
	}
	last->next = next_tok;
	
	if (end_ws)
		last->join_next = false;
	else
		last->join_next = was_join;

	free_str_arr(parts);
	return (last);
}

char	*expand_str(char *s, t_shell *shell)
{
	char	*res;
	int		i;
	bool	sq;
	bool	dq;
	char	*val;

	if (!s)
		return (NULL);
	res = ft_strdup("");
	i = 0;
	sq = false;
	dq = false;
	while (s[i])
	{
		handle_quotes(s[i], &sq, &dq);
		val = get_next_chunk(s, &i, shell, sq, dq);
		res = append_val(res, val);
		free(val);
	}
	free((void *)s);
	return (res);
}

static void join_tokens(t_tok **tokens)
{
	t_tok *cur;
	char *joined;
	t_tok *next;

	cur = *tokens;
	while (cur && cur->next)
	{
		next = cur->next;
		if (cur->type == TOK_WORD && next->type == TOK_WORD && cur->join_next)
		{
			if (!cur->was_quoted && ft_strcmp(cur->value, "$") == 0 && next->was_quoted)
			{
				free(cur->value);
				cur->value = ft_strdup("");
			}
			joined = ft_strjoin(cur->value, next->value);
			free(cur->value);
			cur->value = joined;
			cur->join_next = next->join_next;
			cur->has_wildcard = (cur->has_wildcard || next->has_wildcard);
			cur->was_quoted = (cur->was_quoted || next->was_quoted);
			
			cur->next = next->next;
			free(next->value);
			free(next);
			continue;
		}
		cur = cur->next;
	}
}

void	expand_tokens(t_tok **tokens, t_shell *shell)
{
	t_tok	*cur;
	t_tok	*prev;
	t_tok	*next;
	t_tok	*last;

	cur = *tokens;
	prev = NULL;
	while (cur)
	{
		next = cur->next;
		if (cur->type == TOK_WORD)
		{
			if (prev && prev->type == TOK_HEREDOC) {
				prev = cur;
				cur = next;
				continue;
			}
			expand_tilde_tok(cur, shell);
			if (word_has_wildcard(cur->value) && cur->value[0] == '$')
				 cur->value = expand_str(cur->value, shell);
			else
				cur->value = expand_str(cur->value, shell);
			if (!cur->was_quoted)
				cur->has_wildcard = word_has_wildcard(cur->value);
		}
		prev = cur;
		cur = next;
	}

	cur = *tokens;
	prev = NULL;
	while (cur)
	{
		next = cur->next;
		if (cur->type == TOK_WORD)
		{
			if (prev && prev->type == TOK_HEREDOC) {
				prev = cur;
				cur = next;
				continue;
			}
			// Empty token removal
			if (cur->value && cur->value[0] == '\0' && !cur->was_quoted) 
			{
			    if (prev && !cur->join_next) prev->join_next = false;
				
			    if (prev) prev->next = next;
			    else *tokens = next;
			    free(cur->value);
			    free(cur);
			    cur = next;
			    continue; // proceed with next token (prev stays same)
			}
			if (!cur->was_quoted && has_ifs_space(cur->value))
			{
				last = perform_split_tok(tokens, prev, cur);
				if (last) { prev = last; cur = last->next; }
				else cur = *tokens;
				continue ;
			}
		}
		prev = cur;
		cur = next;
	}
	join_tokens(tokens);
}
