/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 18:50:06 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/11/27 18:54:21 by dbinti-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ========== INCLUDES ========== */
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <errno.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

/* ========== MACROS ========== */
# define SUCCESS 0
# define FAILURE 1
# define TRUE 1
# define FALSE 0

/* ========== ENUMS ========== */

// Token types for lexer
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_ENV_VAR,
	TOKEN_DQUOTE,
	TOKEN_SQUOTE,
	TOKEN_EOF
}	t_token_type;

// Redirection types
typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

/* ========== STRUCTURES ========== */

// Token structure (output of lexer)
typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

// Redirection structure
typedef struct s_redir
{
	t_redir_type	type;
	char			*file;
	int				fd;
	struct s_redir	*next;
}	t_redir;

// Command structure (single command in pipeline)
typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

// Environment variable structure
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

// Main shell structure (global state)
typedef struct s_shell
{
	t_env	*env;
	char	**envp;
	int		exit_status;
	int		stdin_backup;
	int		stdout_backup;
	int		pipe_count;
}	t_shell;

/* ========== GLOBAL VARIABLE ========== */
extern int	g_signal;

/* ========================================================================== */
/*                           FUNCTION PROTOTYPES                              */
/* ========================================================================== */

/* ========== PARSING (Partner A) ========== */

// Lexer (lexer.c)
t_token		*lexer(char *input);
t_token		*create_token(t_token_type type, char *value);
void		add_token(t_token **head, t_token_type type, char *value);
int			is_metachar(char c);
int			is_whitespace(char c);

// Parser (parser.c)
t_cmd		*parser(t_token *tokens, t_shell *shell);
t_cmd		*create_cmd(void);
void		add_arg_to_cmd(t_cmd *cmd, char *arg);
void		add_redir_to_cmd(t_cmd *cmd, t_redir *redir);
int			count_pipes(t_token *tokens);

// Quote handling (quotes.c)
char		*handle_quotes(char *str);
int			has_unclosed_quotes(char *str);
char		*remove_quotes(char *str, char quote_type);
int			is_inside_quotes(char *str, int pos);
int			is_quote(char c);

// Environment variable expansion (expander.c)
char		*expand_vars(char *str, t_shell *shell);
char		*expand_exit_status(char *str, int exit_status);
char		*get_var_name(char *str, int *i);
char		*replace_var(char *str, char *var_name, char *value);
int			should_expand(char *str, int pos);

// Syntax checker (syntax_check.c)
int			check_syntax(t_token *tokens);
int			check_pipes(t_token *tokens);
int			check_redirections(t_token *tokens);
int			syntax_error(char *token);

/* ========== EXECUTION (Partner B) ========== */

// Executor (executor.c)
int			execute_commands(t_cmd *cmd, t_shell *shell);
int			execute_single_cmd(t_cmd *cmd, t_shell *shell);
int			execute_pipeline(t_cmd *cmd, t_shell *shell);
int			execute_external(t_cmd *cmd, t_shell *shell);
void		child_process(t_cmd *cmd, t_shell *shell);

// Pipes (pipes.c)
int			setup_pipes(t_cmd *cmd, t_shell *shell);
int			**create_pipes(int num_pipes);
void		setup_pipe_fds(int cmd_index, int **pipe_fds, int num_pipes);
void		close_pipes(int **pipe_fds, int num_pipes);
void		close_pipe_fds(int **pipe_fds, int num_pipes);

// Redirections (redirections.c)
int			handle_redirections(t_redir *redirs);
int			open_redir_file(t_redir *redir);
int			handle_heredoc(char *delimiter, t_shell *shell);
void		restore_std_fds(t_shell *shell);
int			check_file_permissions(char *file, int mode);

// Signals (signal.c)
void		setup_signals(void);
void		signal_handler(int sig);
void		setup_interactive_signals(void);
void		setup_execution_signals(void);
void		ignore_signals(void);

// Path resolution (path.c)
char		*find_executable(char *cmd, t_env *env);
char		**get_path_dirs(t_env *env);
int			is_executable(char *path);
char		*join_path(char *dir, char *cmd);

/* ========== BUILTINS ========== */

// Builtin utilities (builtin_utils.c)
int			is_builtin(char *cmd);
int			execute_builtin(t_cmd *cmd, t_shell *shell);

// Built-in commands (Partner A)
int			builtin_echo(char **args);
int			builtin_export(char **args, t_shell *shell);
int			builtin_unset(char **args, t_shell *shell);
int			builtin_env(t_shell *shell);

// Built-in commands (Partner B)
int			builtin_cd(char **args, t_shell *shell);
int			builtin_pwd(void);
int			builtin_exit(char **args, t_shell *shell);

/* ========== UTILS (Shared) ========== */

// Environment utilities (env_utils.c)
t_env		*init_env(char **envp);
char		*env_get(char *key, t_env *env);
void		env_set(char *key, char *value, t_env **env);
void		env_unset(char *key, t_env **env);
char		**env_to_array(t_env *env);
void		free_env(t_env *env);
t_env		*create_env_node(char *key, char *value);
int			env_list_size(t_env *env);

// Error handling (error.c)
void		error_exit(char *msg);
void		error_msg(char *cmd, char *msg);
void		perror_prefix(char *cmd);
int			print_error(char *cmd, char *arg, char *msg);
void		syntax_error_msg(char *token);

// Memory management (free.c)
void		free_tokens(t_token *tokens);
void		free_commands(t_cmd *cmd);
void		free_redirs(t_redir *redirs);
void		free_array(char **arr);
void		free_all(t_shell *shell, t_cmd *cmd, t_token *tokens);

/*
** NOTE: All basic string/array utilities are in libft:
** - ft_strdup, ft_strjoin, ft_split, ft_substr
** - ft_strlen, ft_strchr, ft_strncmp, ft_strcmp (if you have it)
** - ft_isalpha, ft_isdigit, ft_isspace (if you have it)
** - ft_atoi, ft_itoa
** - etc.
**
** utils.c should only contain minishell-specific utilities that
** are NOT general-purpose string functions.
*/

#endif

