#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

#define MAX_PLAYER 3
#define NAME_LEN 16
#define PLAYER_INFO_SIZE 4

typedef struct s_player
{
	int		soc;
	char	name[NAME_LEN];
}	t_player;

typedef struct s_game_info
{
	int	stage_num;
	int	player_num;
	int	reached;
	int	crashed;
}	t_game_info;

char		g_players_info[MAX_PLAYER*PLAYER_INFO_SIZE];
t_player	g_players[MAX_PLAYER];
t_game_info	g_game_info;
int			g_nfds;
fd_set		g_mask;

void	recv_data(void);
void	send_data(void);
void	finalize_server_session(void);

void	init_server_session(int	player_num, int	stage_num, int	max_fd)
{
	int		i;
	char	buf[10];

	g_game_info.player_num = player_num;
	g_game_info.stage_num = stage_num;
	g_game_info.reached = 0;
	g_game_info.crashed = 0;
	g_nfds = max_fd + 1;

	FD_ZERO(&g_mask);
	FD_SET(0, &g_mask);
	i = 0;
	while (i < g_game_info.player_num)
	{
		FD_SET(g_players[i].soc, &g_mask);
		i++;
	}

	// レースの情報を全クライアントに送る
	sprintf(buf, "%d %d\n", g_game_info.player_num, g_game_info.stage_num);
	i = 0;
	while (i < g_game_info.player_num)
	{
		write(g_players[i].soc, buf, 10);
		i++;
	}
}

void	loop_server_session(void)
{
	while (g_game_info.crashed + g_game_info.reached < g_game_info.player_num)
	{
		recv_data();
		send_data();
	}
	finalize_server_session();
}

void	recv_data(void)
{
	int		i;
	fd_set	copy_mask;

	copy_mask = g_mask;
	select(g_nfds, &copy_mask, NULL, NULL, NULL);

	i = 0;
	while (i < g_game_info.player_num)
	{
		if (FD_ISSET(g_players[i].soc, &copy_mask))
		{
			read(g_players[i].soc, &g_players_info[i * PLAYER_INFO_SIZE], PLAYER_INFO_SIZE);

			// crash判定

			// goal判定
		}
		i++;
	}

	return ;
}

void	send_data(void)
{
	int		i;
	fd_set	copy_mask;

	i = 0;
	copy_mask = g_mask;
	while (i < g_game_info.player_num)
	{
		if (FD_ISSET(g_players[i].soc, &copy_mask))
		{
			write(g_players[i].soc, g_players_info, PLAYER_INFO_SIZE * g_game_info.player_num);
		}
		i++;
	}
}

void	entry(int	i, int	fd)
{
	char	buf[32];

	g_players[i].soc = fd;
	write(g_players[i].soc, "Type your name: ", 17);
	read(g_players[i].soc, g_players[i].name, NAME_LEN);
	printf("Entry number %d is %s\n", i, g_players[i].name);

	// エントリーナンバーをクライアントに送る
	sprintf(buf, "%d\n", i);
	write(g_players[i].soc, buf, strlen(buf) + 1);
}

void	finalize_server_session(void)
{
	int	i;

	i = 0;
	while (i < g_game_info.player_num)
	{
		close(g_players[i].soc);
	}
}
