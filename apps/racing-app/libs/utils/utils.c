#include "utils.h"

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
	if (bind(soc_waiting, (struct sockaddr *)&me, sizeof(me)) == -1)
	{
		perror("bind");
		return (-1);
	}
	listen(soc_waiting, 1);
	fprintf(stdout, "Please wait for a client to connect\n");
	soc = accept(soc_waiting, NULL, NULL);
	close(soc_waiting);
	return (soc);
}

int	mserver_socket(int	port,	int	backlog)
{
	struct sockaddr_in	me;
	int					soc_waiting;

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

	if (bind(soc_waiting, (struct sockaddr *)&me, sizeof(me)) == -1)
	{
		perror("bind");
		return (-1);
	}

	if (listen(soc_waiting, backlog) == -1)
	{
		perror("listen");
		return (-1);
	}
	fprintf(stdout, "Successfully Setup. Now Waiting Connection\n");

	return (soc_waiting);
}

int	mserver_accept(int	soc_waiting, int	limit, void	(*cb)())
{
	int					i;
	int					fd;

	i = 0;
	while (i < limit)
	{
		fd = accept(soc_waiting, NULL, NULL);
		if (fd == -1)
		{
			perror("accept");
			return (-1);
		}
		(*cb)(i, fd);
		i++;
	}

	close(soc_waiting);
	return (fd);
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
	memcpy(&server.sin_addr, server_ent->h_addr_list[0], server_ent->h_length);

	soc = socket(AF_INET, SOCK_STREAM, 0);
	if (soc < 0)
	{
		perror("socket");
		return (-1);
	}

	if (connect(soc, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		perror("connect");
		return (-1);
	}
	return (soc);
}

char	*chop_newline(char	*str)
{
	int	n;

	n = strlen(str);
	if (str[n - 1] == '\n')
	{
		str[n - 1] = '\0';
	}
	return (str);
}