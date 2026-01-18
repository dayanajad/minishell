/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:54:10 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/12 22:16:04 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

static void	init_shell(t_shell *shell, char **envp)
{
	shell->env = arr_to_env(envp);
	shell->last_status = 0;
	shell->should_exit = 0;
	shell->pids = NULL;
	shell->pid_count = 0;
	shell->heredocs = NULL;
}

static void	process_line(char *line, t_shell *shell)
{
	t_tok	*tokens;
	t_ast	*ast;

	if (!line || !*line)
		return ;
	add_history(line);
	tokens = lexer(line);
	if (!check_syntax(tokens))
	{
		shell->last_status = 258;
		return ;
	}
	expand_tokens(tokens, shell);
	remove_quotes(tokens);
	if (!tokens)
		return ;
	ast = parse(tokens, shell);
	if (ast)
	{
		exec_ast(ast, shell);
		free_ast(ast);
	}
}

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_shell	shell;

	(void)ac;
	(void)av;
	init_shell(&shell, envp);
	setup_signals_interactive();
	while (!shell.should_exit)
	{
		g_signal = 0;
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (g_signal == SIGINT)
			shell.last_status = 130;
		process_line(line, &shell);
		free(line);
	}
	free_env(shell.env);
	return (shell.last_status);
}
