#ifndef UTILS_H
# define UTILS_H
# include <netinet/in.h>
# include <netdb.h>
# include <stdio.h>
# include <string.h>
# include <sys/socket.h>
# include <unistd.h>
int		setup_server(int	port);
int		mserver_socket(int	port,	int	backlog);
int		mserver_accept(int	soc_waiting, int	limit, void	(*cb)());
int		setup_client(int	port, char	*hostname);
char	*chop_newline(char	*str);
#endif
