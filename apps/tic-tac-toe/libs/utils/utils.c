#include "libs/utils/utils.h"

int	setup_server(int	port)
{
	struct sockaddr_in	me;
	int					soc_waiting;
	int					soc;

	memset(&me, 0, sizeof(me));
	me.sin_family = AF_INET;
	me.sin_addr.s_addr = htonl(INADDR_ANY);
	me.sin_port = htons(port);

	soc_waiting = socket(AF_INET, SOCK_STREAM, 0);
	if (soc_waiting < 0)
	{
		perror("socket");
		return (-1);
	}
	listen(soc_waiting, 1);
	fprintf(stdout, "Please wait for a client to connect\n");
	soc = accept(soc_waiting, NULL, NULL);
	close(soc_waiting);
	return (soc);
}

int	setup_client(int	port, char	*hostname)
{
	struct hostent		*server_ent;
	struct sockaddr_in	server;
	int					soc;

	server_ent = gethostbyname(hostname);
	if (server_ent == NULL)
	{
		perror("gethostbyname");
		return (-1);
	}

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	
}