/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:54:10 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/12/08 18:45:20 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <string.h>
# include <unistd.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/ioctl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <errno.h>
# include <term.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>

extern volatile sig_atomic_t	g_signal;

typedef struct s_ast			t_ast;

// TOKEN

typedef enum e_tok_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_AND,
	TOK_OR,
	TOK_IN,
	TOK_OUT,
	TOK_APP,
	TOK_HEREDOC,
	TOK_LPAREN,
	TOK_RPAREN,
	TOK_END
}	t_tok_type;

typedef struct s_tok
{
	t_tok_type		type;
	char			*value;
	bool			has_wildcard;
	bool			was_quoted;
	int				pos;
	struct s_tok	*next;
}	t_tok;

// REDIRECTIONS

typedef enum e_redir_type
{
	R_IN,
	R_OUT,
	R_APP,
	R_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*target;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char	**av;
	t_redir	*redirs;
}	t_cmd;

// ABSTRACT SYNTAX TREE (AST)

typedef enum e_ast_type
{
	A_CMD,
	A_PIPE,
	A_AND,
	A_OR,
	A_SUBSHELL
}	t_ast_type;

typedef struct s_binary
{
	t_ast	*left;
	t_ast	*right;
}	t_binary;

typedef struct s_ast
{
	t_ast_type	type;
	union
	{
		t_cmd		*cmd;
		t_binary	branch;
		t_ast		*child;
	} u_data;
}	t_ast;

// ENVIRONMENT

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

// HEREDOC STRUCT

typedef struct s_hdoc
{
	char			*path;
	struct s_hdoc	*next;
}	t_hdoc;

// SHELL

typedef struct s_shell
{
	t_env	*env;
	int		last_status;
	int		should_exit;
	int		*pids;
	int		pid_count;
	t_hdoc	*heredocs;
}	t_shell;

// srcs/utils/free.c

void	free_str_arr(char **arr);
void	free_env(t_env *env);
void	free_redirs(t_redir *redirs);
void	free_cmd(t_cmd *cmd);
void	free_ast(t_ast *ast);

// srcs/parsing/parser.c

t_ast	*parse(t_tok *tokens);
t_ast	*parse_or(t_tok **cur);
t_ast	*parse_and(t_tok **cur);
t_ast	*parse_pipe(t_tok **cur);

// srcs/parsing/parse_command.c

t_ast	*parse_subshell(t_tok **cur);

// srcs/parsing/parse_utils.c

bool	is_cmd_end(t_tok *tok);
bool	av_push(char ***avp, const char *word);
void	syntax_err_tok(t_tok *tok);

// srcs/parsing/parse_redir.c

bool	parse_one_redir(t_tok **cur, t_redir **redirs);

// srcs/parsing/ast_create.c

t_cmd	*new_cmd(void);
t_ast	*new_ast_cmd(t_cmd *cmd);
t_ast	*new_ast_bin(t_ast_type type, t_ast *left, t_ast *right);
t_ast	*new_ast_subshell(t_ast	*child);
t_redir	*new_redir(t_redir_type type, const char *target);

// srcs/utils/env_utils.c

char	*get_env_value(t_env *env, char *key);
t_env	*arr_to_env(char **envp);

// srcs/utils/env_arr.c

char	**env_to_array(t_env *env);

// srcs/execution/path.c

char	*find_in_path(char *cmd, t_env *env);

// srcs/execution/executor.c

int		exec_ast(t_ast *ast, t_shell *shell);

// srcs/execution/exec_logic.c

int		exec_and_node(t_ast *ast, t_shell *shell);
int		exec_or_node(t_ast *ast, t_shell *shell);
int		exec_subshell_node(t_ast *ast, t_shell *shell);

// srcs/execution/redirections.c

int		save_stdio(int saved[2]);
int		restore_stdio(int saved[2]);
bool	apply_redirections(t_redir *redirs);

// srcs/execution/pipes.c

int		exec_pipe_node(t_ast *node, t_shell *shell);

// srcs/execution/heredoc.c

bool	process_heredocs(t_ast *ast, t_shell *shell);

// srcs/execution/heredoc_utils.c

int		heredoc_create_file(t_redir *redir, int index, t_shell *shell);
void	cleanup_heredoc_files(t_shell *shell);

// srcs/execution/signal.c

void	set_parent_signals(void);
void	set_child_signals(void);
void	set_heredoc_signals(void);
void	disable_ctrl_c_echo(void);

// srcs/builtins/builtin_utils.c

bool	is_builtin(const char *name);
int		exec_builtin(t_cmd *cmd, t_shell *shell);

// srcs/builtins/echo.c

int		builtin_echo(char **av);

// srcs/builtins/cd.c

int		builtin_cd(char **av, t_shell *shell);

// srcs/builtins/pwd_env.c

int		builtin_pwd(void);
int		builtin_env(t_env *env);

// srcs/builtins/export.c

int		builtin_export(char **av, t_env **env);

// srcs/builtins/export_utils.c

int		export_one_arg(char *arg, t_env **env);

// srcs/builtins/unset.c

int		builtin_unset(char **av, t_env **env);

// srcs/builtins/exit.c

int		builtin_exit(char **av, t_shell *shell);

#endif
