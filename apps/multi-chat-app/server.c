#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include "server_session.h"

int	main(int	argc, char	*argv[])
{
	int	attendant_num;
	int	soc_waiting;
	int	max_fd;

	(void)argv;
	if (argc != 2)
	{
		fprintf(stderr, "引数には会議への参加人数を入力してください\n");
		exit(1);
	}

	attendant_num = atoi(argv[1]);

	soc_waiting = mserver_socket(PORT, attendant_num);
	if (soc_waiting == -1)
		exit(1);
	max_fd = mserver_accept(soc_waiting, attendant_num, init_attendant);

	init_server_session(attendant_num, max_fd);
	loop_server_session();
}
