NAME        := minishell
BONUS_NAME  := minishell_bonus

CC          := cc
CFLAGS      := -Wall -Wextra -Werror

INC_DIR     := includes
LIBFT_DIR   := libft
LIBFT       := $(LIBFT_DIR)/libft.a

CPPFLAGS    := -I$(INC_DIR) -I$(LIBFT_DIR)

OBJ_DIR     := obj

SRCS        := srcs/main.c \
               srcs/main_filter.c \
               srcs/main_finalize.c \
               srcs/main_init.c \
               srcs/main_line.c \
               srcs/main_line_utils.c \
               srcs/main_process.c \
               srcs/main_run.c \
               srcs/builtins/builtin_utils.c \
               srcs/builtins/builtin_utils_helpers.c \
               srcs/builtins/cd.c \
               srcs/builtins/echo.c \
               srcs/builtins/echo_utils.c \
               srcs/builtins/exit.c \
               srcs/builtins/export.c \
               srcs/builtins/export_apply.c \
               srcs/builtins/export_print.c \
               srcs/builtins/export_print_utils.c \
               srcs/builtins/export_utils.c \
               srcs/builtins/pwd_env.c \
               srcs/builtins/unset.c \
               srcs/execution/exec_bg.c \
               srcs/execution/exec_bg_helpers.c \
               srcs/execution/exec_bg_utils.c \
               srcs/execution/exec_logic.c \
               srcs/execution/exec_pipe.c \
               srcs/execution/exec_pipe_utils.c \
               srcs/execution/exec_resolve.c \
               srcs/execution/exec_subshell.c \
               srcs/execution/exec_utils.c \
               srcs/execution/exec_utils_helpers.c \
               srcs/execution/exec_utils_wait.c \
               srcs/execution/executor.c \
               srcs/execution/heredoc.c \
               srcs/execution/heredoc_utils.c \
               srcs/execution/path.c \
               srcs/execution/redir_utils.c \
               srcs/execution/redirections.c \
               srcs/execution/signal_state.c \
               srcs/execution/signals.c \
               srcs/parsing/ast_create.c \
               srcs/parsing/expander.c \
               srcs/parsing/expander_ansi.c \
               srcs/parsing/expander_ansi_utils.c \
               srcs/parsing/expander_backslash.c \
               srcs/parsing/expander_helpers.c \
               srcs/parsing/expander_join.c \
               srcs/parsing/expander_loop.c \
               srcs/parsing/expander_split.c \
               srcs/parsing/expander_tok_split.c \
               srcs/parsing/expander_tok_split_utils.c \
               srcs/parsing/expander_utils.c \
               srcs/parsing/lexer.c \
               srcs/parsing/lexer_ops.c \
               srcs/parsing/lexer_ops_utils.c \
               srcs/parsing/lexer_scan.c \
               srcs/parsing/lexer_utils.c \
               srcs/parsing/lexer_word.c \
               srcs/parsing/lexer_word_utils.c \
               srcs/parsing/parse_command.c \
               srcs/parsing/parse_redir.c \
               srcs/parsing/parse_redir_heredoc.c \
               srcs/parsing/parse_redir_utils.c \
               srcs/parsing/parse_utils.c \
               srcs/parsing/parser.c \
               srcs/parsing/parser_err.c \
               srcs/parsing/parser_or_helpers.c \
               srcs/parsing/parser_seq.c \
               srcs/parsing/quotes.c \
               srcs/parsing/syntax_check.c \
               srcs/parsing/syntax_check_utils.c \
               srcs/parsing/syntax_utils.c \
               srcs/parsing/tok_repr_utils.c \
               srcs/parsing/wildcard_match.c \
               srcs/parsing/wildcard_utils.c \
               srcs/parsing/wildcards.c \
               srcs/utils/arr_env.c \
               srcs/utils/env_arr.c \
               srcs/utils/env_utils.c \
               srcs/utils/error.c \
               srcs/utils/free.c \
               srcs/utils/utils.c \
               srcs/utils/utils_read.c

OBJS        := $(SRCS:srcs/%.c=$(OBJ_DIR)/%.o)
DEPS        := $(OBJS:.o=.d)

LDFLAGS     :=
LDLIBS      := $(LIBFT) -lreadline -lhistory -lncurses

RM          := rm -rf

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: srcs/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -MP -c $< -o $@

bonus: $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(BONUS_NAME)

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME) $(BONUS_NAME)

re: fclean all

-include $(DEPS)

.PHONY: all bonus clean fclean re
