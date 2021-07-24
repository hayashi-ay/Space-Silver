#ifndef SERVER_SESSION_H
# define SERVER_SESSION_H
# define MAX_ATTENDANT 5
# define MAX_NAME_LEN 16
# define BUF_LEN 80
# define PORT 9999
# include <netdb.h>
# include <stdio.h>
# include <string.h>
# include <sys/select.h>
# include <sys/socket.h>
# include <unistd.h>
# include "utils.h"
void	init_attendant(int	i, int	fd);
void	init_server_session(int	num, int	max_fd);
void	loop_server_session(void);
#endif
