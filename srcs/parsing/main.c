/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 13:00:18 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/12/23 13:57:39 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int ac, char **av, char **envp)
{
    t_shell shell;
    t_ast   *ast;
    t_cmd   *cmd;

    (void)ac;
    (void)av;
    (void)envp;

    // Initialize shell
    shell.env = NULL;
    shell.last_status = 0;
    shell.should_exit = 0;
    shell.pids = NULL;
    shell.pid_count = 0;
    shell.heredocs = NULL;

    printf("Shell initialized!\n");

    // Create a simple command: echo hello
    cmd = new_cmd();
    av_push(&cmd->av, "echo");
    av_push(&cmd->av, "hello");
    // No redirections, so cmd->redirs stays NULL

    // Wrap it in an AST node
    ast = new_ast_cmd(cmd);

    printf("Created AST for: echo hello\n");
    printf("Command: %s\n", ast->u_data.cmd->av[0]);
    printf("Arg[1]: %s\n", ast->u_data.cmd->av[1]);

    // TODO: Execute the AST

    // Cleanup
    free_ast(ast);

    return (0);
}
