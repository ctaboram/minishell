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
EXP_DIR		= export

SRCS		=	$(SRC_DIR)/main.c \
				$(SRC_DIR)/prompt.c \
				$(SRC_DIR)/ft_cd.c \
				$(SRC_DIR)/ft_echo.c \
				$(SRC_DIR)/ft_pwd.c \
				$(SRC_DIR)/ft_env.c \
				$(SRC_DIR)/$(EXP_DIR)/ft_export.c \
				$(SRC_DIR)/$(EXP_DIR)/ft_utils_export.c \
				$(SRC_DIR)/ft_unset.c \
				$(SRC_DIR)/tokenizacion.c \


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
	if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(LIBFT_A):
	@echo "$(YELLOW)Compiling libft...$(RESET)"
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR)
	@echo "$(GREEN)Compilation complete$(RESET)"

clean:
	@echo "$(RED)Eliminando obj...$(RESET)"
	if exist "$(OBJ_DIR)" rmdir /s /q "$(OBJ_DIR)"
	$(MAKE) --no-print-directory -C $(LIBFT_DIR) clean

fclean: clean
	@echo "$(RED)Eliminando $(NAME)...$(RESET)"
	if exist "$(NAME)" del "$(NAME)"
	$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re