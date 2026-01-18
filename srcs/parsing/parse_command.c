/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 17:58:54 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/12/08 17:58:58 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	fill_simple_cmd(t_tok **cur, t_cmd *cmd, t_shell *shell)
{
	t_tok	*tok;

	tok = *cur;
	while (tok && !is_cmd_end(tok))
	{
		if (tok->type == TOK_WORD)
		{
			if (!av_push(&cmd->av, tok->value))
				return (0);
			*cur = tok->next;
		}
		else if (parse_one_redir(cur, &cmd->redirs, shell) == false)
			return (0);
		tok = *cur;
	}
	return (1);
}

static t_ast	*parse_simple_cmd(t_tok **cur, t_shell *shell)
{
	t_cmd	*cmd;

	cmd = new_cmd();
	if (!cmd)
		return (NULL);
	if (!fill_simple_cmd(cur, cmd, shell))
	{
		free_cmd(cmd);
		return (NULL);
	}
	if (!cmd->av && !cmd->redirs)
	{
		syntax_err_tok(*cur);
		free_cmd(cmd);
		return (NULL);
	}
	return (new_ast_cmd(cmd));
}

t_ast	*parse_subshell(t_tok **cur, t_shell *shell)
{
	t_tok	*tok;
	t_ast	*child;

	tok = *cur;
	if (tok && tok->type == TOK_LPAREN)
	{
		*cur = tok->next;
		child = parse_or(cur, shell);
		if (!child)
			return (NULL);
		tok = *cur;
		if (!tok || tok->type != TOK_RPAREN)
		{
			syntax_err_tok(tok);
			free_ast(child);
			return (NULL);
		}
		*cur = tok->next;
		return (new_ast_subshell(child));
	}
	return (parse_simple_cmd(cur, shell));
}
