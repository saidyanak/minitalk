# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/02 11:11:29 by syanak            #+#    #+#              #
#    Updated: 2026/02/02 13:23:01 by syanak           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME_S = server
NAME_C = client

NAME_B_S = server_bonus
NAME_B_C = client_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror

all: $(NAME_S) $(NAME_C)

bonus: $(NAME_B_S) $(NAME_B_C)

$(NAME_B_C): client_bonus.c
	$(CC) $(CFLAGS) -o $(NAME_B_C) client_bonus.c

$(NAME_B_S): server_bonus.c
	$(CC) $(CFLAGS) -o $(NAME_B_S) server_bonus.c

$(NAME_S): server.c
	$(CC) $(CFLAGS) -o $(NAME_S) server.c

$(NAME_C): client.c
	$(CC) $(CFLAGS) -o $(NAME_C) client.c

clean:
	rm -f $(NAME_S) $(NAME_C)
	@if [ -f $(NAME_B_S) ]; then rm -f $(NAME_B_S); fi
	@if [ -f $(NAME_B_C) ]; then rm -f $(NAME_B_C); fi

fclean: clean
	

re: fclean all

.PHONY: all clean fclean re bonus cleanbonus
