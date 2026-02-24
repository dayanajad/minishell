/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:10:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 03:05:57 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	prepare_tokens(t_tok **tokens, t_shell *shell)
{
	if (!check_syntax(*tokens))
	{
		shell->last_status = 2;
		if (!isatty(STDIN_FILENO))
			shell->should_exit = 1;
		return (0);
	}
	expand_tokens(tokens, shell);
	expand_wildcards(tokens);
	remove_quotes(*tokens);
	return (1);
}

static t_tok	*lex_and_prepare(char *line, t_shell *shell)
{
	t_tok	*tokens;

	tokens = lexer(line);
	if (!tokens)
	{
		shell->last_status = 2;
		if (!isatty(STDIN_FILENO))
			shell->should_exit = 1;
		return (NULL);
	}
	if (!prepare_tokens(&tokens, shell))
		return (NULL);
	if (!tokens || tokens->type == TOK_END)
	{
		free_tokens(tokens);
		return (NULL);
	}
	return (tokens);
}

static void	execute_and_free(t_ast *ast, t_tok *tokens, t_shell *shell)
{
	if (ast)
	{
		exec_ast(ast, shell);
		free_ast(ast);
	}
	else
	{
		shell->last_status = 2;
		if (!isatty(STDIN_FILENO))
			shell->should_exit = 1;
	}
	free_tokens(tokens);
}

static int	is_blank_line(const char *line)
{
	int	i;

	i = 0;
	while (line[i] && ft_strchr(" \t\r\n", line[i]))
		i++;
	return (line[i] == '\0');
}

void	process_line(char *line, t_shell *shell)
{
	t_tok	*tokens;
	t_ast	*ast;
	int		len;

	if (!line)
		return ;
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	if (is_blank_line(line))
		return ;
	if (isatty(STDIN_FILENO))
		add_history(line);
	shell->heredoc_eof = 0;
	tokens = lex_and_prepare(line, shell);
	if (!tokens)
		return ;
	ast = parse(tokens, shell);
	execute_and_free(ast, tokens, shell);
}
