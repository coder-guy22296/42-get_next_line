#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cyildiri <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/10/28 22:37:41 by cyildiri          #+#    #+#              #
#    Updated: 2016/11/02 22:32:08 by cyildiri         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

CFLAGS += -Wall -Wextra
LDFLAGS += -Wall -Wextra
CFLAGS += -I libft/includes

CFLAGS += -Werror
CFLAGS += -Werror

all:
	make -C libft/ fclean && make -C libft/ && make -C libft/ clean
	clang -Wall -Wextra -Werror -I libft/includes -o get_next_line.o -c get_next_line.c
	clang -Wall -Wextra -Werror -I libft/includes -o main.o -c main.c
	clang -o test_gnl main.o get_next_line.o -I libft/includes -L libft/ -lft

test:
	make -C libft/ fclean && make -C libft/ && make -C libft/ clean
	clang $(CFLAGS) -o get_next_line.o -c get_next_line.c
	clang $(CFLAGS) -o main.o -c main.c
	clang $(LDFLAGS) -o test_gnl main.o get_next_line.o -I libft/includes -L libft/ -lft
