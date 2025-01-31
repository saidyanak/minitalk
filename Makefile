# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: syanak <syanak@student.42kocaeli.com.tr >  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/31 19:08:06 by syanak            #+#    #+#              #
#    Updated: 2025/01/31 19:08:56 by syanak           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minitalk
BNAME = bonus_minitalk

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC = client.c server.c
SRCNAME_CLIENT = client
SRCNAME_SERVER = server

BSOURCES = server_bonus.c client_bonus.c
BOBJECTS = $(BSOURCES:.c=.o)
OBJECTS = $(SRC:.c=.o)

all: $(SRCNAME_CLIENT) $(SRCNAME_SERVER)

$(SRCNAME_CLIENT): client.o
	$(CC) $(CFLAGS) client.o -o $(SRCNAME_CLIENT)

$(SRCNAME_SERVER): server.o
	$(CC) $(CFLAGS) server.o -o $(SRCNAME_SERVER)

bonus: $(BNAME)

$(BNAME): $(BOBJECTS)
	$(CC) $(CFLAGS) $(BOBJECTS) -o $(SRCNAME_CLIENT) $(SRCNAME_SERVER)

clean:
	rm -rf $(OBJECTS) $(BOBJECTS)

fclean: clean
	rm -rf $(SRCNAME_CLIENT) $(SRCNAME_SERVER) $(BNAME)

re: fclean all

.PHONY: all clean fclean re bonus
