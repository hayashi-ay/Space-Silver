CC = gcc
CFLAGS = -Wall -Wextra -Werror
LIBDIR = libs/utils
LIBS = $(LIBDIR)/utils.a

build:
	$(CC) $(CFLAGS) -I$(LIBDIR) -o server.o server.c games.c $(LIBS)
	$(CC) $(CFLAGS) -I$(LIBDIR) -o client.o client.c games.c $(LIBS)

clean:
	rm client.o server.o

re:	clean build

.PHONY: build clean re
