CC = gcc
CFLAGS = -Wall -Wextra -Werror

build:
	$(CC) $(CFLAGS) -o server.o server.c
	$(CC) $(CFLAGS) -o client.o client.c

clean:
	rm client.o server.o

re:	clean build

.PHONY: build clean re
