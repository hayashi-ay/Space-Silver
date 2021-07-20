CC = gcc
CFLAGS = -Wall -Wextra -Werror
LIBDIR = libs/utils
LIBS = $(LIBDIR)/utils.a

build:
	$(CC) $(CFLAGS) -o server.o server.c tag.c -I$(LIBDIR) $(LIBS) -lcurses
	$(CC) $(CFLAGS) -o client.o client.c tag.c -I$(LIBDIR) $(LIBS) -lcurses

clean:
	rm client.o server.o

re:	clean build

.PHONY: build clean re
