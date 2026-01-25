/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpichyal <bpichyal@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 12:10:00 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/25 18:44:40 by bpichyal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_pwd_and_shlvl(t_shell *shell)
{
	char	cwd[4096];
	char	*val;
	int		lvl;
	char	*lvl_s;

	if (!get_env_value(shell->env, "PWD"))
	{
		if (getcwd(cwd, sizeof(cwd)) != NULL)
			export_set_env_var(&shell->env, "PWD", cwd);
	}
	if (!export_find_env_node(shell->env, "OLDPWD"))
		export_set_env_var(&shell->env, "OLDPWD", NULL);
	val = get_env_value(shell->env, "SHLVL");
	if (!val || val[0] == '\0')
		lvl = 0;
	else
		lvl = ft_atoi(val);
	lvl++;
	lvl_s = ft_itoa(lvl);
	if (!lvl_s)
		return ;
	export_set_env_var(&shell->env, "SHLVL", lvl_s);
	free(lvl_s);
}

void	init_shell(t_shell *shell, char **envp)
{
	shell->env = arr_to_env(envp);
	init_pwd_and_shlvl(shell);
	shell->last_status = 0;
	shell->should_exit = 0;
	shell->pids = NULL;
	shell->pid_count = 0;
	shell->heredocs = NULL;
	shell->in_child = 0;
	shell->heredoc_eof = 0;
	shell->noninteractive_prompt_newline = 0;
	shell->stdout_filter_pid = -1;
}
