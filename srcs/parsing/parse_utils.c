/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 18:13:28 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/12/08 18:13:33 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_cmd_end(t_tok *tok)
{
	if (!tok)
		return (true);
	if (tok->type == TOK_END || tok->type == TOK_PIPE
		|| tok->type == TOK_AND || tok->type == TOK_OR
		|| tok->type == TOK_AMP || tok->type == TOK_SEMI
		|| tok->type == TOK_RPAREN)
		return (true);
	if (tok->type == TOK_PIPE_AMP)
		return (true);
	return (false);
}

void	syntax_err_tok(t_tok *tok)
{
	const char	*val;

	val = tok_repr(tok);
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	ft_putstr_fd((char *)val, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}

static bool	av_set_word(char ***avp, char **tmp, size_t len,
		const char *word)
{
	tmp[len] = ft_strdup(word);
	if (!tmp[len])
	{
		perror("minishell: strdup");
		*avp = tmp;
		return (false);
	}
	tmp[len + 1] = NULL;
	*avp = tmp;
	return (true);
}

bool	av_push(char ***avp, const char *word)
{
	char	**tmp;
	size_t	len;

	if (!word)
		return (false);
	len = 0;
	if (*avp)
	{
		while ((*avp)[len])
			len++;
	}
	tmp = realloc(*avp, sizeof(*tmp) * (len + 2));
	if (!tmp)
	{
		perror("minishell: realloc");
		return (false);
	}
	return (av_set_word(avp, tmp, len, word));
}
