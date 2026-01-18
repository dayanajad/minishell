NAME        = minishell

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -Iincludes -Ilibft
RM          = rm -f

SRCS_DIR    = srcs

SRCS        = $(SRCS_DIR)/main.c \
              $(SRCS_DIR)/builtins/builtin_utils.c \
              $(SRCS_DIR)/builtins/cd.c \
              $(SRCS_DIR)/builtins/echo.c \
              $(SRCS_DIR)/builtins/exit.c \
              $(SRCS_DIR)/builtins/export.c \
              $(SRCS_DIR)/builtins/pwd_env.c \
              $(SRCS_DIR)/builtins/unset.c \
              $(SRCS_DIR)/execution/exec_logic.c \
              $(SRCS_DIR)/execution/exec_pipe.c \
              $(SRCS_DIR)/execution/exec_subshell.c \
              $(SRCS_DIR)/execution/exec_utils.c \
              $(SRCS_DIR)/execution/executor.c \
              $(SRCS_DIR)/execution/heredoc.c \
              $(SRCS_DIR)/execution/path.c \
              $(SRCS_DIR)/execution/redirections.c \
              $(SRCS_DIR)/execution/signals.c \
              $(SRCS_DIR)/parsing/ast_create.c \
              $(SRCS_DIR)/parsing/expander.c \
              $(SRCS_DIR)/parsing/expander_utils.c \
              $(SRCS_DIR)/parsing/lexer.c \
              $(SRCS_DIR)/parsing/lexer_utils.c \
              $(SRCS_DIR)/parsing/parse_command.c \
              $(SRCS_DIR)/parsing/parse_redir.c \
              $(SRCS_DIR)/parsing/parse_utils.c \
              $(SRCS_DIR)/parsing/parser.c \
              $(SRCS_DIR)/parsing/quotes.c \
              $(SRCS_DIR)/parsing/syntax_check.c \
              $(SRCS_DIR)/utils/arr_env.c \
              $(SRCS_DIR)/utils/env_arr.c \
              $(SRCS_DIR)/utils/env_utils.c \
              $(SRCS_DIR)/utils/error.c \
              $(SRCS_DIR)/utils/free.c \
              $(SRCS_DIR)/utils/utils.c

OBJS        = $(SRCS:.c=.o)

LIBFT_DIR   = libft
LIBFT       = $(LIBFT_DIR)/libft.a

READLINE_LIB = -lreadline

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(READLINE_LIB) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C $(LIBFT_DIR) clean
	$(RM) $(OBJS)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
