.SILENT:
NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
LDFLAGS		= -lreadline
INCLUDE_DIR	= includes
LIBFT_DIR	= libft
LIBFT_A		= $(LIBFT_DIR)/libft.a

SRC_DIR		= src
OBJ_DIR		= obj

SRCS		=	$(SRC_DIR)/main.c \
				$(SRC_DIR)/prompt.c \
				$(SRC_DIR)/token.c \

OBJS		=	$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

MINISHELL_H	= $(INCLUDE_DIR)/minishell.h
LIBFT_H		= $(INCLUDE_DIR)/libft.h

IFLAGS		= -I$(INCLUDE_DIR) -I$(LIBFT_DIR)

RED			= \033[1;31m
GREEN		= \033[1;32m
YELLOW		= \033[1;33m
RESET		= \033[0m

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
	@echo "$(YELLOW)Compiling minishell...$(RESET)"
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) $(LDFLAGS) $(IFLAGS) -o $(NAME)
	@echo "$(GREEN)Compilation complete$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(MINISHELL_H) $(LIBFT_H)
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(LIBFT_A):
	@echo "$(YELLOW)Compiling libft...$(RESET)"
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR)
	@echo "$(GREEN)Compilation complete$(RESET)"

clean:
	@echo "$(RED)Eliminando obj...$(RESET)"
	rm -rf $(OBJ_DIR)
	$(MAKE) --no-print-directory -C $(LIBFT_DIR) clean

fclean: clean
	@echo "$(RED)Eliminando $(NAME)...$(RESET)"
	rm -f $(NAME)
	$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re