/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbinti-m <dbinti-m@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:54:10 by dbinti-m          #+#    #+#             */
/*   Updated: 2025/12/08 01:01:49 by dbinti-m         ###   ########.fr       */
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
	t_redir_type		type;
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
	} data;
}	t_ast;

// ENVIRONMENT

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

// SHELL

typedef struct s_shell
{
	t_env	*env;
	int		last_status;
	int		should_exit;
	int		*pids;
	int		pid_count;
}	t_shell;

// srcs/utils/free.c

void	free_str_arr(char **arr);
void	free_env(t_env *env);

// srcs/utils/env_utils.c

char	*get_env_value(t_env *env, char *key);
t_env	*arr_to_env(char **envp);

// srcs/execution/path.c

char *find_in_path(char *cmd, t_env *env);

#endif
