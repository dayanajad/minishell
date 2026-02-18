/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 19:11:08 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/12 19:28:08 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	set_signal(SIGINT);
	write(STDOUT_FILENO, "\n", 1);
	if (get_readline_active())
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_sigpipe(int sig)
{
	(void)sig;
	exit(128 + SIGPIPE);
}

void	setup_signals_interactive(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
}

void	setup_signals_exec(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
}
