NAME_S = server
NAME_C = client

CC = cc
CFLAGS = -Wall -Wextra -Werror

all: $(NAME_S) $(NAME_C)

$(NAME_S): server.c
	$(CC) $(CFLAGS) -o $(NAME_S) server.c

$(NAME_C): client.c
	$(CC) $(CFLAGS) -o $(NAME_C) client.c

clean:
	rm -f $(NAME_S) $(NAME_C)

fclean: clean

re: fclean all

.PHONY: all clean fclean re
