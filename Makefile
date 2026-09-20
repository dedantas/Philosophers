# ========== VARIABLES ==========
NAME            = philo
CC              = cc
SRCDIR          = src
INCDIR          = includes
CFLAGS          = -Wall -Wextra -Werror -pthread -g -I$(INCDIR)
RM              = rm -f

# ========== COLORS ==========
RED             = \033[0;31m
GREEN           = \033[0;32m
YELLOW          = \033[0;33m
BLUE            = \033[0;34m
MAGENTA         = \033[0;35m
CYAN            = \033[0;36m
RESET           = \033[0m

# ========== SOURCES ==========
SRCS            = main.c utils.c init.c simulation.c actions.c cleanup.c
SRCS_PATH       = $(addprefix $(SRCDIR)/, $(SRCS))
OBJS            = $(SRCS_PATH:.c=.o)

# ========== RULES ==========
all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) compiled successfully$(RESET)"

$(SRCDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/philo.h
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(CYAN)Compiling $<...$(RESET)"

clean:
	@$(RM) $(OBJS)
	@echo "$(YELLOW)Object files removed$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED)Executable removed$(RESET)"

re: fclean all

# ========== TEST RULES ==========
test: all
	@echo "$(MAGENTA)Running tests...$(RESET)"
	@echo "$(BLUE)Test 1: 5 philosophers$(RESET)"
	./$(NAME) 5 800 200 200
	@echo "\n$(BLUE)Test 2: With meal limit$(RESET)"
	./$(NAME) 4 410 200 200 5
	@echo "\n$(BLUE)Test 3: Single philosopher$(RESET)"
	./$(NAME) 1 800 200 200
	@echo "\n$(GREEN)✓ All tests completed$(RESET)"

debug: CFLAGS += -g -fsanitize=thread
debug: re
	@echo "$(MAGENTA)Debug version compiled with ThreadSanitizer$(RESET)"

.PHONY: all clean fclean re test debug
