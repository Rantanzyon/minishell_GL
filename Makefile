CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCDIR = src
MDIR = $(SRCDIR)/main
LDIR = $(SRCDIR)/lexer
PDIR = $(SRCDIR)/parser
EDIR = $(SRCDIR)/exec
SRC = $(addprefix $(MDIR)/, \
	main.c \
	prompt.c \
	data_creation.c \
	data_destroy.c \
	data_init.c \
	) \
	$(addprefix $(LDIR)/, \
	lexer.c \
	lexer_quote.c \
	lexer_exp.c \
	lexer_exp_utils.c \
	lexer_quote_final.c \
	lexer_final.c \
	lexer_final_utils.c \
	) \
	$(addprefix $(PDIR)/, \
	parser.c \
	syntax_error.c \
	rename_tok.c \
	ast.c \
	) \
	src/print/print.c \
#	ast.c \
#	src/print/print_tree.c

OBJ = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC))
OBJ_DIR = obj
NAME = minishell
INCLUDE = include
LIBFT_DIR = libft
LIBFT = libft/libft.a

FILE_C := $(shell find src -type f | wc -l)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@
	@$(eval FILE_O := $(shell find obj -type f 2>/dev/null | wc -l))
	@printf "$(YEL)\rMinishell [%d%%]$(DEF)" $$(($(FILE_O) * 100 / $(FILE_C)))

all : $(NAME)

$(NAME) : $(OBJ)
	@printf "\r$(YEL)Minishell [100%%]$(DEF)"
	@echo
	@make -C $(LIBFT_DIR) --no-print-directory
	@$(CC) $(CFLAGS) $^ -o $@ $(LIBFT) -lreadline
	@echo "$(GRN)Compiled.$(DEF)"

clean :
	@rm -rf $(OBJ)
	@rm -rf $(OBJ_DIR)
	@rm -f test/*.o
	@make clean -C $(LIBFT_DIR) --no-print-directory
	@echo "$(CYN)Object files cleaned.$(DEF)"

fclean : clean
	@rm -f $(NAME)
	@make fclean -C $(LIBFT_DIR) --no-print-directory
	@echo "$(CYN)Executable files cleaned.$(DEF)"

re : fclean all

run : re
	@./$(NAME)

color :
	@./test.sh

.PHONY : all clean fclean re test

.SILENT:

ifeq ($(shell uname), Linux)
    RED = \e[0;91m
    GRN = \e[0;92m
    YEL = \e[0;93m
	CYN = \e[0;96m
	DEF = \e[0m
else
    RED = \033[0;31m
    GRN = \033[0;92m
    YEL = \033[0;93m
	CYN = \033[0;96m
	DEF = \033[0m
	BGYEL = \033[30;43m
	BG = \033[30;45m
endif


