#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int	main(void)
{
	const in_port_t		PORT = 9999;
	const int			BUF_LEN = 512;
	char				*hostname = "localhost";
	struct hostent		*server_ent;
	struct sockaddr_in	server;
	int					soc;
	char				buf[BUF_LEN];
	int					bytes;

	server_ent = gethostbyname(hostname);
	if (server_ent == NULL)
	{
		perror("gethostbyname");
		exit(1);
	}

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	memcpy(&server.sin_addr, server_ent->h_addr, server_ent->h_length);

	soc = socket(AF_INET, SOCK_STREAM, 0);
	if (soc < 0)
	{
		perror("socket");
		exit(1);
	}

	if (connect(soc, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		perror("connect");
		exit(1);
	}

	write(1, "Wait\n", 5);

	while (1)
	{
		bytes = read(soc, buf, BUF_LEN);
		write(1, buf, bytes);
		bytes = read(0, buf, BUF_LEN);
		write(soc, buf, bytes);
		if (strncmp(buf, "quit", 4) == 0)
		{
			break ;
		}
	}
	close(soc);
}
