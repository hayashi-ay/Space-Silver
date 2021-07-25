NAME = utils.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror
OBJS = utils.o
RM = rm
AR = ar -rc

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

all: $(NAME)

fclean: clean
	$(RM) $(NAME)

clean:
	$(RM) $(OBJS)

re: fclean all

.PHONY: all fclean clean re
