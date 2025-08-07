# Program name
NAME		= minishell

# Compiler and flags
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g3
DEBUG_FLAGS	= -DDEBUG_MODE=1
INCLUDES	= -I. -I./lexer -I./gc -I./utils -I./parser -I./debugging -I./executor -I./signal

# Readline flags
READLINE	= -lreadline

# Directories
SRCDIR		= .
OBJDIR		= obj
LEXERDIR	= lexer
PARSERDIR	= parser
EXECUTEDIR	= executor
GCDIR		= gc
SIGNDIR		= signals
UTILSDIR	= utils
DEBUGDIR	= debugging

# Source files
SRCS		= minishell.c \
			  $(LEXERDIR)/lexer.c \
			  $(LEXERDIR)/expand_variables.c \
			  $(LEXERDIR)/lexer_utils.c \
			  $(LEXERDIR)/lexer_tools.c \
			  $(GCDIR)/gc.c \
			  $(UTILSDIR)/utils1.c \
			  $(UTILSDIR)/utils2.c \
			  $(UTILSDIR)/utils3.c \
			  $(UTILSDIR)/utils4.c \
			  $(UTILSDIR)/utils5.c \
			  $(UTILSDIR)/utils6.c \
			  $(UTILSDIR)/utils7.c \
			  $(UTILSDIR)/validation_sort.c \
			  $(PARSERDIR)/parser.c \
			  $(PARSERDIR)/parser_tools.c \
			  $(PARSERDIR)/parse_simple.c \
			  $(PARSERDIR)/wildcard.c \
			  $(EXECUTEDIR)/executor.c \
			  $(EXECUTEDIR)/executor_utils2.c \
			  $(EXECUTEDIR)/executor_utils.c \
			  $(EXECUTEDIR)/builtins.c \
			  $(EXECUTEDIR)/builtins_functions.c \
			  $(EXECUTEDIR)/builtin_cd.c \
			  $(DEBUGDIR)/debugging_tools_1.c \
			  $(DEBUGDIR)/debugging_tools_2.c \
			  $(SIGNDIR)/signals.c

# Object files
OBJS		= $(SRCS:%.c=$(OBJDIR)/%.o)

# Header dependencies
HEADERS		= minishell.h \
			  $(LEXERDIR)/lexer.h \
			  $(GCDIR)/gc.h \
			  $(UTILSDIR)/utils.h \
			  $(PARSERDIR)/parser.h \
			  $(EXECUTEDIR)/executor.h \
			  $(DEBUGDIR)/debugging_tools.h \
			  $(SIGNDIR)/signals.h

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

debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(NAME)

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

debug_re: fclean debug

.PHONY: all debug clean fclean re debug_re
