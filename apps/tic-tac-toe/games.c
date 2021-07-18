#include "games.h"

const int	g_port = 9999;
char		g_server_stone;
char		g_client_stone;
int			g_soc;

void	init_game(int	soc, char	server_stone, char	client_stone)
{
	g_soc = soc;
	g_server_stone = server_stone;
	g_client_stone = client_stone;
}

void	finalize_game(void)
{
	close(g_soc);
}
