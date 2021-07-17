#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int	main(void)
{
	const in_port_t		PORT = 9999;
	const int			BUF_LEN = 512;
	struct sockaddr_in	me;
	int					soc_waiting;
	int					soc;
	char				buf[BUF_LEN];
	int					bytes;

	memset(&me, 0, sizeof(me));
	me.sin_family = AF_INET;
	me.sin_addr.s_addr = htonl(INADDR_ANY);
	me.sin_port = htons(PORT);

	soc_waiting = socket(AF_INET, SOCK_STREAM, 0);
	if (soc_waiting < 0)
	{
		perror("socket");
		exit(1);
	}
	if (bind(soc_waiting, (struct sockaddr *)&me, sizeof(me)) == -1)
	{
		perror("bind");
		exit(1);
	}
	listen(soc_waiting, 1);
	soc = accept(soc_waiting, 0, 0);
	close(soc_waiting);

	write(1, "Go ahead!\n", 10);

	while (1)
	{
		bytes = read(0, buf, BUF_LEN);
		write(soc, buf, bytes);
		bytes = read(soc, buf, BUF_LEN);
		write(1, buf, bytes);
		if (strncmp(buf, "quit", 4) == 0)
		{
			break ;
		}
	}
	close(soc);
}
