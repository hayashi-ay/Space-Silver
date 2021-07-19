#include "games.h"
#include <stdlib.h>

int	main(void)
{
	int			soc;
	const char	server_stone = 'o';
	const char	client_stone = 'x';
	const int	port = 9999;

	soc = setup_server(port);
	if (soc == -1)
	{
		exit(1);
	}
	fprintf(stdout, "Successfully Connected!\n");
	init_game(soc, client_stone, server_stone);

	while (1)
	{
		show_game();
		fprintf(stdout, "Peer turn\n");
		if (peer_turn() == -1)
			break ;
		show_game();
		fprintf(stdout, "Your turn\n");
		if (my_turn() == -1)
			break ;
	}

	finalize_game();
	fprintf(stdout, "Thank you for playing!\n");
}
