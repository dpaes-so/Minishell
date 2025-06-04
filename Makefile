# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dgarcez- <dgarcez-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/11 16:46:46 by dgarcez-          #+#    #+#              #
#    Updated: 2025/06/04 13:43:38 by dgarcez-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

LIBFT = ./incs/libft/libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

SOURCES = minishell.c \
		  parser/parsing.c \
          parser/tokenization_utils.c \
          parser/tokenization.c \
          parser/check_tokens.c \
          parser/expansions.c \
		  parser/dollar_exp.c \
		  parser/exp_options.c \
		  parser/expansions_utils.c \
		  parser/new_tokens_utils.c \
		  parser/final_split.c \
		  parser/quotes_funcs.c \
		  parser/create_array.c \
		  parser/ast_utils.c \
		  parser/ast_tree.c \
		  parser/ast_creation_utils.c \
          built-ins/mini_built_in.c \
          built-ins/mini_cd.c \
          built-ins/mini_env.c \
          built-ins/mini_export.c \
		  built-ins/mini_export_utils.c \
		  built-ins/mini_export_more_utils.c \
		  built-ins/mini_export_even_more_utils.c \
		  built-ins/mini_you_know_it_more_export_utils.c \
		  built-ins/mini_unset.c \
		  built-ins/mini_exit.c \
		  built-ins/mini_echo.c \
		  exec/mini_exec.c \
		  exec/mini_humans.c \
		  exec/mini_aux_exec.c \
		  exec/mini_more_aux_exec.c \
		  exec/mini_signal.c \
		  exec/mini_random_efunc.c \
		  exec/mini_here_doc.c \
		  exec/fdaniel.c \
          mini_aux.c

SRCS_DIR = srcs
SRCS =	$(addprefix $(SRCS_DIR)/, $(SOURCES))

OBJS_DIR = objs
OBJ_FILES =  $(addprefix $(OBJS_DIR)/, $(SOURCES:.c=.o))

all: $(NAME)

$(NAME): $(OBJ_FILES) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJ_FILES) $(LIBFT) -lreadline -o $(NAME)
	@echo DONE!

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	@echo COMPILING...
	@$(CC) $(CFLAGS)  -c $< -o $@

$(LIBFT):
	@make -C ./incs/libft -s
	
$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(OBJS_DIR)/built-ins
	@mkdir -p $(OBJS_DIR)/parser
	@mkdir -p $(OBJS_DIR)/exec

val: $(NAME)
	valgrind --leak-check=full --track-fds=yes --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp -s ./minishell

#--trace-children=yes   

clean:
	@make clean -C ./incs/libft -s
	@rm -rf $(OBJS_DIR)

fclean: clean
	make fclean -C ./incs/libft -s
	rm -f $(NAME)

re: fclean all