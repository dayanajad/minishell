/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:54:10 by dbinti-m          #+#    #+#             */
/*   Updated: 2026/01/12 22:16:04 by dbinti-m         ###   ########.fr       */
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

sig_atomic_t	get_signal(void);
void			set_signal(int sig);

typedef struct s_ast	t_ast;

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
	TOK_PIPE_AMP,
	TOK_AMP,
	TOK_SEMI,
	TOK_END
}	t_tok_type;

typedef struct s_tok
{
	t_tok_type		type;
	char			*value;
	bool			has_wildcard;
	bool			was_quoted;
	int				pos;
	bool			join_next;
	bool			from_glob;
	struct s_tok	*next;
}	t_tok;

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
	int				fd;
	char			*target;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char	**av;
	t_redir	*redirs;
	t_tok	*raw_toks;
}	t_cmd;

typedef enum e_ast_type
{
	A_CMD,
	A_PIPE,
	A_AND,
	A_OR,
	A_SEQ,
	A_SUBSHELL,
	A_BG
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

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_hdoc
{
	char			*path;
	struct s_hdoc	*next;
}	t_hdoc;

typedef struct s_shell
{
	t_env	*env;
	int		last_status;
	int		should_exit;
	int		*pids;
	int		pid_count;
	t_hdoc	*heredocs;
	int		in_child;
	int		heredoc_eof;
	int		noninteractive_prompt_newline;
	pid_t	stdout_filter_pid;
}	t_shell;

typedef struct s_pipe_flags
{
	bool	use_pipe;
	bool	keep_pipe_open;
}	t_pipe_flags;

typedef struct s_pipe_state
{
	int		pipefd[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		status;
	int		status_left;
	bool	right_has_stdin_redir;
	bool	right_is_builtin;
}	t_pipe_state;

typedef struct s_chunk_ctx
{
	t_shell	*sh;
	bool	sq;
	bool	dq;
}	t_chunk_ctx;

char			*read_line_nobuf(int fd);
void			init_shell(t_shell *shell, char **envp);
void			setup_stdout_nul_filter(t_shell *shell);
int				is_blank_line(const char *line);
void			sanitize_line(char *line);
char			*read_input_line(t_shell *shell);
void			process_line(char *line, t_shell *shell);
void			finalize_shell(t_shell *shell);
int				run_shell(t_shell *shell);
void			free_str_arr(char **arr);
void			free_env(t_env *env);
void			free_redirs(t_redir *redirs);
void			free_cmd(t_cmd *cmd);
void			free_ast(t_ast *ast);
void			free_tokens(t_tok *tokens);
char			*get_env_value(t_env *env, char *key);
t_env			*arr_to_env(char **envp);
char			**env_to_arr(t_env *env);
t_ast			*parse(t_tok *tokens, t_shell *shell);
t_ast			*parse_or(t_tok **cur, t_shell *shell);
t_ast			*parse_and(t_tok **cur, t_shell *shell);
t_ast			*parse_pipe(t_tok **cur, t_shell *shell);
t_ast			*parse_err(t_ast *left);
t_ast			*parse_or_token(t_tok **cur, t_shell *shell, t_ast *left);
t_ast			*parse_amp_token(t_tok **cur, t_shell *shell, t_ast *left);
t_ast			*parse_subshell(t_tok **cur, t_shell *shell);
bool			is_cmd_end(t_tok *tok);
bool			av_push(char ***avp, const char *word);
void			syntax_err_tok(t_tok *tok);
const char		*tok_repr(t_tok *tok);
const char		*tok_repr_op(t_tok_type type);
bool			parse_one_redir(t_tok **cur, t_redir **redirs, t_shell *shell);
char			*unescape_backslashes(const char *s);
void			append_redir(t_redir **redirs, t_redir *new);
t_cmd			*new_cmd(void);
t_ast			*new_ast_cmd(t_cmd *cmd);
t_ast			*new_ast_bin(t_ast_type type, t_ast *left, t_ast *right);
t_ast			*new_ast_subshell(t_ast	*child);
t_redir			*new_redir(t_redir_type type, int fd, const char *target);
int				exec_cmd_node(t_ast *ast, t_shell *shell);
int				exec_ast(t_ast *ast, t_shell *shell);
int				exec_and_node(t_ast *ast, t_shell *shell);
int				exec_or_node(t_ast *ast, t_shell *shell);
int				exec_seq_node(t_ast *ast, t_shell *shell);
int				exec_bg_node(t_ast *ast, t_shell *shell);
int				exec_bg_open_pipe(bool capture, int outpipe[2]);
void			exec_bg_close_pipe(int outpipe[2]);
void			exec_bg_flush_output(int fd);
void			exec_bg_child(t_ast *ast, t_shell *sh, int p[2], bool c);
int				exec_bg_parent(t_ast *ast, t_shell *sh, int p[2], pid_t pid);
int				exec_pipe_node(t_ast *ast, t_shell *shell);
bool			exec_pipe_cmd_has_input(t_ast *ast);
bool			exec_pipe_is_builtin_cmd(t_ast *ast);
int				exec_subshell_node(t_ast *ast, t_shell *shell);
int				save_stdio(int saved[3]);
int				restore_stdio(int saved[3]);
bool			apply_redirections(t_redir *redirs, t_shell *shell);
bool			resolve_redir_target(t_redir *r, char **target, char ***m);
bool			dup_and_close(int fd, int dest);
void			handle_sigint(int sig);
void			setup_signals_interactive(void);
void			setup_signals_exec(void);
char			*read_heredoc(const char *delim, t_shell *shell, bool expand);
int				open_temp_file(char **path);
void			warn_heredoc_eof(const char *delimiter);
char			*find_in_path(char *cmd, t_env *env);
int				normalize_status(int status);
void			exec_external_child(t_cmd *cmd, t_shell *shell);
int				exec_external_cmd(t_cmd *cmd, t_shell *shell);
bool			exec_suppress_error(t_shell *shell);
void			exec_print_cmd_not_found(t_shell *shell, char *cmd);
void			exec_print_is_dir(t_shell *shell, char *path);
void			exec_as_script(char *path, t_cmd *cmd, char **envp);
void			exec_error_exit(t_shell *shell, char *path);
int				exec_wait_for_child(pid_t pid);
bool			is_builtin(const char *name);
int				exec_builtin(t_cmd *cmd, t_shell *shell);
bool			echo_suppresses_newline(char **av);
int				exec_builtin_echo(t_cmd *cmd, t_shell *shell);
int				builtin_echo(char **av);
int				write_all(int fd, const char *s, size_t len);
int				echo_write_str(const char *s);
int				echo_write_ch(char c);
int				write_echo_args(char **av, int i);
int				builtin_cd(char **av, t_shell *shell);
int				builtin_pwd(void);
int				builtin_env(t_env *env);
int				builtin_export(char **av, t_env **env);
bool			export_is_valid_identifier(const char *s);
t_env			*export_find_env_node(t_env *env, const char *key);
void			export_set_env_var(t_env **env, char *key, char *value);
int				builtin_unset(char **av, t_env **env);
int				builtin_exit(char **av, t_shell *shell);
t_tok			*new_tok(t_tok_type type, char *value, int pos);
void			tok_add(t_tok **head, t_tok *new);
int				is_meta(char c);
t_tok			*lexer(char *s);
int				lex_word(char *s, int i, t_tok **list);
int				lex_op(char *s, int i, t_tok **list);
int				lex_redir_op(char *s, int i, t_tok **list);
int				lex_misc_op(char *s, int i, t_tok **list);
int				scan_to_comment_or_eof(char *s, char *quote);
t_tok			*lex_tokens(char *s);
bool			check_syntax(t_tok *tokens);
bool			is_op_token(t_tok_type type);
bool			is_redir_token(t_tok_type type);
bool			check_op_seq(t_tok *cur);
bool			check_redir_seq(t_tok *cur);
void			expand_tokens(t_tok **tokens, t_shell *shell);
char			*expand_str(char *s, t_shell *shell);
char			*expand_var(char *s, int *i, t_shell *shell);
char			*get_next_chunk(char *s, int *i, t_chunk_ctx *ctx);
char			*expand_heredoc_str(char *s, t_shell *shell);
void			expand_wildcards(t_tok **tokens);
char			**get_matches(const char *pattern);
void			insert_matches(t_tok **cur, char **matches);
void			remove_quotes(t_tok *tokens);
char			*append_val(char *res, char *val);
void			handle_quotes(char c, bool *in_sq, bool *in_dq);
char			*process_char(char c);
int				ft_error(char *msg);
void			perror_msg(char *msg);

#endif
