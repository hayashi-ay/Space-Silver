#include <stdio.h>
#include <stdlib.h>
#include "race.h"
#include "server_session.h"
#include "utils.h"

int	main(int	argc, char	*argv[])
{
	int	player_num;
	int	stage_num;
	int	soc_waiting;
	int	max_fd;

	if (argc != 3)
	{
		fprintf(stderr, "Invalid argument\n");
		exit(1);
	}
	player_num = atoi(argv[1]);
	stage_num = atoi(argv[2]);

	soc_waiting = mserver_socket(PORT, player_num);
	if (soc_waiting == -1)
	{
		exit(1);
	}
	max_fd = mserver_accept(soc_waiting, player_num, entry);
	init_server_session(player_num, stage_num, max_fd);
	loop_server_session();
}
