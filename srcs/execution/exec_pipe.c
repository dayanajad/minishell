/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 20:27:38 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/12 21:09:59 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipeline.h"

static int	init_pipeline(t_pipeline *st, t_ast *ast)
{
	int	index;

	st->count = exec_pipe_count(ast);
	st->output = -1;
	st->status = 1;
	st->stages = malloc(sizeof(*st->stages) * st->count);
	st->pids = ft_calloc(st->count, sizeof(*st->pids));
	if (!st->stages || !st->pids)
	{
		free(st->stages);
		free(st->pids);
		return (0);
	}
	index = 0;
	exec_pipe_collect(ast, st->stages, &index);
	return (1);
}

static int	spawn_stage(t_pipeline *st, t_shell *shell, int index)
{
	int	pipefd[2];

	pipefd[0] = -1;
	pipefd[1] = -1;
	if (index > 0 && pipe(pipefd) < 0)
		return (perror("minishell: pipe"), 0);
	st->pids[index] = fork();
	if (st->pids[index] == 0)
		exec_pipe_child(st->stages[index], shell, st->output, pipefd);
	if (pipefd[0] >= 0)
		close(pipefd[0]);
	if (st->pids[index] < 0)
	{
		if (pipefd[1] >= 0)
			close(pipefd[1]);
		return (perror("minishell: fork"), 0);
	}
	if (st->output >= 0)
		close(st->output);
	st->output = pipefd[1];
	return (1);
}

static void	reap_pipeline(t_pipeline *st, int flags)
{
	int		index;
	int		status;
	pid_t	pid;

	index = -1;
	while (++index < st->count)
	{
		if (st->pids[index] <= 0)
			continue ;
		pid = waitpid(st->pids[index], &status, flags);
		while (pid < 0 && errno == EINTR)
			pid = waitpid(st->pids[index], &status, flags);
		if (pid > 0)
		{
			if (index == st->count - 1)
				st->status = normalize_status(status);
			st->pids[index] = 0;
		}
	}
}

static int	finish_pipeline(t_pipeline *st, int failed)
{
	int	index;

	if (st->output >= 0)
		close(st->output);
	index = -1;
	while (failed && ++index < st->count)
	{
		if (st->pids[index] > 0)
			kill(st->pids[index], SIGTERM);
	}
	reap_pipeline(st, 0);
	free(st->stages);
	free(st->pids);
	if (failed)
		return (1);
	return (st->status);
}

int	exec_pipe_node(t_ast *ast, t_shell *shell)
{
	t_pipeline	st;
	int			index;
	int			failed;

	if (!init_pipeline(&st, ast))
		return (1);
	index = st.count;
	failed = 0;
	while (--index >= 0 && !failed)
	{
		reap_pipeline(&st, WNOHANG);
		failed = !spawn_stage(&st, shell, index);
	}
	return (finish_pipeline(&st, failed));
}
