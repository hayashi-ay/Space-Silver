CC = gcc
CFLAGS = -Wall -Wextra -Werror

build:
	$(CC) $(CFLAGS) -o server server.c
	$(CC) $(CFLAGS) -o client client.c

clean:
	rm client server

re:	clean build

.PHONY: build clean re
