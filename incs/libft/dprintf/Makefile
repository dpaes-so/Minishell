# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dpaes-so <dpaes-so@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/04 12:46:52 by dpaes-so          #+#    #+#              #
#    Updated: 2024/11/04 14:03:09 by dpaes-so         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftdprintf.a

CC = cc
FLAGS = -Wall -Werror -Wextra

SRC_FILES = ft_fdputcharf.c  ft_fdputnbrbasef.c  ft_fdputnbrf.c ft_fdputnbrf_unsigned.c \
		ft_fdputstrf.c  ft_fdputptr.c ft_fdprintf.c

OBJS = $(SRC_FILES:.c=.o)

all: $(NAME) 

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	
%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)
	
re: fclean all

.PHONY: all fclean clean re
