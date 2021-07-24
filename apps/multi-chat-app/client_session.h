#ifndef CLIENT_SESSION_H
# define CLIENT_SESSION_H
# define BUF_LEN 80
# define SEND_WIN_WIDTH 60
# define SEND_WIN_HEIGHT 1
# define RECV_WIN_WIDTH 60
# define RECV_WIN_HEIGHT 13
# include <curses.h>
# include <netdb.h>
# include <signal.h>
# include <stdio.h>
# include <string.h>
# include <sys/select.h>
# include <sys/socket.h>
# include <unistd.h>
# include "utils.h"
void	init_client_session(int	soc);
void	loop_client_session(void);
void	setup_client_window(void);
#endif
