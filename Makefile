NAME := minishell
BONUS_NAME := minishell_bonus

CC := cc
CFLAGS := -Wall -Wextra -Werror

INC_DIR := includes
LIBFT_DIR := libft
LIBFT := $(LIBFT_DIR)/libft.a

CPPFLAGS := -I$(INC_DIR) -I$(LIBFT_DIR)

OBJ_DIR := obj

SRCS := $(shell find srcs -type f -name '*.c')
OBJS := $(SRCS:srcs/%.c=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

LDFLAGS :=
LDLIBS := $(LIBFT) -lreadline -lhistory -lncurses

RM := rm -f

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: srcs/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -MP -c $< -o $@

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(NAME)
	ln -sf $(NAME) $(BONUS_NAME)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(RM) $(NAME) $(BONUS_NAME)

re: fclean all

-include $(DEPS)

.PHONY: all bonus clean fclean re