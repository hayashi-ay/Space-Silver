CC = gcc
CFLAGS = -Wall -Wextra -Werror

LIBDIR = libs/utils
LIBS = $(LIBDIR)/utils.a

build: server client

server:
	$(CC) $(CFLAGS) -o server.o server.c server_session.c -I$(LIBDIR) $(LIBS)

client:
	$(CC) $(CFLAGS) -o client.o client.c client_session.c -I$(LIBDIR) $(LIBS) -lcurses

clean:
	rm server.o client.o

re:	clean build

.PHONY: server clean re
