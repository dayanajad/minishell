/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 18:30:02 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/12 22:09:14 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir_type	redir_type_from_tok(t_tok_type type)
{
	if (type == TOK_IN)
		return (R_IN);
	if (type == TOK_OUT)
		return (R_OUT);
	if (type == TOK_APP)
		return (R_APP);
	return (R_HEREDOC);
}

static int	default_redir_fd(t_redir_type type)
{
	if (type == R_IN || type == R_HEREDOC)
		return (STDIN_FILENO);
	return (STDOUT_FILENO);
}

static bool	add_redir_target(t_tok **cur, t_redir_type type, int fd,
		t_redir **redirs)
{
	t_tok	*target;

	target = *cur;
	if (!target || target->type != TOK_WORD)
	{
		syntax_err_tok(target);
		return (false);
	}
	append_redir(redirs, new_redir(type, fd, target->value));
	*cur = target->next;
	return (true);
}

static int	check_io_number(t_tok **cur)
{
	t_tok	*io;
	int		fd;

	io = *cur;
	if (io && io->type == TOK_WORD && io->next
		&& is_all_digits(io->value)
		&& (io->pos + (int)ft_strlen(io->value) == io->next->pos)
		&& (io->next->type == TOK_IN || io->next->type == TOK_OUT
			|| io->next->type == TOK_APP || io->next->type == TOK_HEREDOC))
	{
		fd = ft_atoi(io->value);
		*cur = io->next;
		return (fd);
	}
	return (-1);
}

bool	parse_one_redir(t_tok **cur, t_redir **redirs, t_shell *shell)
{
	t_tok			*op;
	t_redir_type	type;
	int				fd;
	t_tok			*save;

	save = *cur;
	fd = check_io_number(cur);
	op = *cur;
	if (!op || (op->type != TOK_IN && op->type != TOK_OUT
			&& op->type != TOK_APP && op->type != TOK_HEREDOC))
	{
		if (fd != -1)
			*cur = save;
		return (false);
	}
	type = redir_type_from_tok(op->type);
	if (fd == -1)
		fd = default_redir_fd(type);
	*cur = op->next;
	if (type == R_HEREDOC)
		return (handle_heredoc_redir(cur, redirs, shell, fd));
	return (add_redir_target(cur, type, fd, redirs));
}
