# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/28 23:45:17 by glemaire          #+#    #+#              #
#    Updated: 2024/03/31 16:39:14 by glemaire         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
CC = gcc
FLAGS = -Wall -Wextra -Werror
INCLUDE = include

SRC = 	$(addprefix src/std/, \
		ft_atoi.c \
		ft_bzero.c \
		ft_calloc.c \
		ft_findchar.c \
		ft_isalnum.c \
		ft_isalpha.c \
		ft_isascii.c \
		ft_isdigit.c \
		ft_isprint.c \
		ft_itoa.c \
		ft_memchr.c \
		ft_memcmp.c \
		ft_memcpy.c \
		ft_memmove.c \
		ft_memset.c \
		ft_putchar_fd.c \
		ft_putendl_fd.c \
		ft_putnbr_fd.c \
		ft_putstr_fd.c \
		ft_split.c \
		ft_strchr.c \
		ft_strdup.c \
		ft_striteri.c \
		ft_strjoin.c \
		ft_strlcat.c \
		ft_strlcpy.c \
		ft_strlen.c \
		ft_strmapi.c \
		ft_strcmp.c \
		ft_strncmp.c \
		ft_strnstr.c \
		ft_strrchr.c \
		ft_strtrim.c \
		ft_substr.c \
		ft_tolower.c \
		ft_toupper.c \
		) \
		$(addprefix src/get_next_line/, \
		get_next_line.c \
		get_next_line_utils.c \
		) \
		$(addprefix src/ft_printf/, \
		format_char.c \
		format_nbr.c \
		format_str.c \
		ft_printf.c \
		) \
		$(addprefix src/lst/, \
		ft_lstnew.c \
		ft_lstadd_front.c \
		ft_lstsize.c \
		ft_lstlast.c \
		ft_lstadd_back.c \
		ft_lstdelone.c \
		ft_lstclear.c \
		ft_lstiter.c \
		ft_lstmap.c \
		ft_lstmove.c \
		ft_lstdelnode.c \
		ft_lstdel_n.c \
		ft_lstdel_here.c \
		ft_lstfusion.c \
		ft_lstadd_at.c \
		ft_lst_insert_lst.c \
		ft_lstindex.c \
		ft_lstat.c \
		)

OBJ = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC))
OBJ_DIR = obj

FILE_O = 0
FILE_C = $(words $(filter %.c,$(SRC)))

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@
	$(eval FILE_O := $(shell echo $$(($(FILE_O) + 1))))
	@printf "$(YEL)\rLibft [%d%%]$(DEF)" $$(($(FILE_O) * 100 / $(FILE_C)))

$(NAME): $(OBJ)
	@ar -rc $(NAME) $(OBJ)
	@echo

all: $(NAME)
	@printf "\r$(YEL)Libft [100%%]$(DEF)"

clean:
	@rm -rf $(addprefix $(OBJ_DIR)/,$(OBJ))
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: fclean clean all re

.SILENT:

ifeq ($(shell uname), Linux)
    RED = \e[0;91m
    GRN = \e[0;92m
    YEL = \e[0;93m
	DEF = \e[0m
else
    RED = \033[0;91m
    GRN = \033[0;92m
    YEL = \033[0;93m
	DEF = \033[0m
endif