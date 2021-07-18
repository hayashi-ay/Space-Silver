#ifndef UTILS_H
# define UTILS_H
# include <netinet/in.h>
# include <netdb.h>
# include <stdio.h>
# include <string.h>
# include <sys/socket.h>
int	setup_server(int	port);
int	setup_client(int	port, char	*hostname);
#endif
