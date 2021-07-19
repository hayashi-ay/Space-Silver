#include "games.h"

const int	g_port = 9999;
char		g_peer_stone;
char		g_my_stone;
int			g_soc;
char		g_board[3][3];

void	init_game(int	soc, char	peer_stone, char	my_stone)
{
	g_soc = soc;
	g_peer_stone = peer_stone;
	g_my_stone = my_stone;
	memset(&g_board, '.', 9);
}

void	show_game(void)
{
	int	i;
	int	j;

	i = 0;
	while (i < 3)
	{
		j = 0;
		while (j < 3)
		{
			write(1, &g_board[i][j], 1);
			j++;
		}
		write(1, "\n", 1);
		i++;
	}
}

int	peer_turn(void)
{
	char	buf[10];
	int		x;
	int		y;

	read(g_soc, buf, 10);

	if (buf[0] == 'q')
	{
		return (-1);
	}
	x = buf[0] - 'a';
	y = buf[1] - '0' - 1;
	g_board[x][y] = g_peer_stone;
	return (1);
}

int	my_turn(void)
{
	char	buf[10];
	int		x;
	int		y;
	
	while (1)
	{
		read(0, buf, 10);
		if (buf[0] == 'q')
		{
			write(g_soc, buf, 10);
			return (-1);
		}
		if (buf[0] < 'a' || buf[0] > 'c')
		{
			fprintf(stdout, "The vertical position is between a and c\n");
			continue ;
		}
		if (buf[1] < '1' || buf[1] > '3')
		{
			fprintf(stdout, "The horizontal position is between 1 and 3\n");
			continue ;
		}
		break ;
	}
	x = buf[0] - 'a';
	y = buf[1] - '0' - 1;
	g_board[x][y] = g_my_stone;

	write(g_soc, buf, 10);
	return (1);
}

void	finalize_game(void)
{
	close(g_soc);
}
