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
		|| tok->type == TOK_RPAREN)
		return (true);
	return (false);
}

static const char	*tok_repr(t_tok *tok)
{
	if (!tok || tok->type == TOK_END)
		return ("newline");
	if (tok->type == TOK_PIPE)
		return ("|");
	if (tok->type == TOK_AND)
		return ("&&");
	if (tok->type == TOK_OR)
		return ("||");
	if (tok->type == TOK_LPAREN)
		return ("(");
	if (tok->type == TOK_RPAREN)
		return (")");
	if (tok->type == TOK_IN)
		return ("<");
	if (tok->type == TOK_OUT)
		return (">");
	if (tok->type == TOK_APP)
		return (">>");
	if (tok->type == TOK_HEREDOC)
		return ("<<");
	if (tok->type == TOK_WORD && tok->value)
		return (tok->value);
	return ("token");
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

static size_t	av_len(char **av)
{
	size_t	len;

	len = 0;
	if (!av)
		return (0);
	while (av[len])
		len++;
	return (len);
}

bool	av_push(char ***avp, const char *word)
{
	char	**av;
	size_t	len;

	if (!word)
		return (false);
	av = *avp;
	len = av_len(av);
	av = (char **)realloc(av, sizeof(char *) * (len + 2));
	if (!av)
	{
		perror("minishell: realloc");
		return (false);
	}
	av[len] = ft_strdup(word);
	if (!av[len])
	{
		perror("minishell: strdup");
		return (false);
	}
	av[len + 1] = NULL;
	*avp = av;
	return (true);
}
