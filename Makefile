# Program name
NAME		= minishell

# Compiler and flags
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g3
INCLUDES	= -I. -I./lexer -I./gc -I./utils

# Readline flags
READLINE	= -lreadline

# Directories
SRCDIR		= .
OBJDIR		= obj
LEXERDIR	= lexer
GCDIR		= gc
UTILSDIR	= utils

# Source files
SRCS		= minishell.c \
			  $(LEXERDIR)/lexer.c \
			  $(LEXERDIR)/expand_variables.c \
			  $(LEXERDIR)/lexer_utils.c \
			  $(LEXERDIR)/lexer_tools.c \
			  $(GCDIR)/gc.c \
			  $(UTILSDIR)/utils1.c

# Object files
OBJS		= $(SRCS:%.c=$(OBJDIR)/%.o)

# Header dependencies
HEADERS		= minishell.h \
			  $(LEXERDIR)/lexer.h \
			  $(GCDIR)/gc.h \
			  $(UTILSDIR)/utils.h

# Colors for prettier output
RED			= \033[0;31m
GREEN		= \033[0;32m
YELLOW		= \033[0;33m
BLUE		= \033[0;34m
PURPLE		= \033[0;35m
CYAN		= \033[0;36m
WHITE		= \033[0;37m
RESET		= \033[0m

# Rules
all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(YELLOW)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(READLINE)
	@echo "$(GREEN)✓ $(NAME) compiled successfully!$(RESET)"

$(OBJDIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	@echo "$(CYAN)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@if [ -d "$(OBJDIR)" ]; then \
		echo "$(RED)Removing object files...$(RESET)"; \
		rm -rf $(OBJDIR); \
		echo "$(GREEN)✓ Object files cleaned!$(RESET)"; \
	fi

fclean: clean
	@if [ -f "$(NAME)" ]; then \
		echo "$(RED)Removing $(NAME)...$(RESET)"; \
		rm -f $(NAME); \
		echo "$(GREEN)✓ $(NAME) removed!$(RESET)"; \
	fi

re: fclean all

.PHONY: all clean fclean re
