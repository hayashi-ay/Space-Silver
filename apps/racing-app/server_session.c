#include "server_session.h"

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
char		g_ranking_info[MAX_PLAYER*RANKING_INFO_SIZE];
t_player	g_players[MAX_PLAYER];
t_game_info	g_game_info;
int			g_nfds;
fd_set		g_mask;

void	recv_data(void);
void	send_data(int	all);
void	init_data(void);
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

	init_data();
	send_data(1);
}

void	loop_server_session(void)
{
	while (g_game_info.crashed + g_game_info.reached < g_game_info.player_num)
	{
		printf("crashed: %d, reached: %d, player: %d\n", g_game_info.crashed, g_game_info.reached, g_game_info.player_num);
		fflush(stdout);
		recv_data();
		send_data(0);
	}
	printf("crashed: %d, reached: %d, player: %d\n", g_game_info.crashed, g_game_info.reached, g_game_info.player_num);
	fflush(stdout);
	finalize_server_session();
}

void	recv_data(void)
{
	int		i;
	int		ranking;
	char	damage;
	char	stage;
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
			damage = g_players_info[i * PLAYER_INFO_SIZE + OFFSET_DAMAGE];
			if (damage >= MAX_DAMAGE)
			{
				ranking = g_game_info.player_num - g_game_info.crashed - 1;
				strcpy(&g_ranking_info[ranking * RANKING_INFO_SIZE + OFFSET_NAME], g_players[i].name);
				g_game_info.crashed++;
			}
			// goal判定
			stage = g_players_info[i * PLAYER_INFO_SIZE + OFFSET_STAGE];
			printf("stage: %d\n", stage);
			fflush(stdout);
			if (stage > (char)g_game_info.stage_num)
			{
				ranking = g_game_info.reached;
				strcpy(&g_ranking_info[ranking * RANKING_INFO_SIZE + OFFSET_NAME], g_players[i].name);
				g_game_info.reached++;
			}
		}
		
		i++;
	}
	i = 0;
	while (i < 4)
	{
		printf("%d\n", g_players_info[i]);
		fflush(stdout);
		i++;
	}
}

void	send_data(int	all)
{
	int		i;
	fd_set	copy_mask;

	i = 0;
	copy_mask = g_mask;
	while (i < g_game_info.player_num)
	{
		if (all || FD_ISSET(g_players[i].soc, &copy_mask))
		{
			write(g_players[i].soc, g_players_info, PLAYER_INFO_SIZE * g_game_info.player_num);
		}
		i++;
	}
	i = 0;
	while (i < 4)
	{
		printf("%d\n", g_players_info[i]);
		fflush(stdout);
		i++;
	}
}

void	init_data(void)
{
	int	i;

	i = 0;
	while (i < g_game_info.player_num)
	{
		g_players_info[i * PLAYER_INFO_SIZE + OFFSET_STAGE] = (char)1;
		g_players_info[i * PLAYER_INFO_SIZE + OFFSET_X] = (char)(2 + i);
		g_players_info[i * PLAYER_INFO_SIZE + OFFSET_Y] = (char)20;
		g_players_info[i * PLAYER_INFO_SIZE + OFFSET_DAMAGE] = (char)0;
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
		write(g_players[i].soc, g_ranking_info, RANKING_INFO_SIZE * g_game_info.player_num);
		i++;
	}

	i = 0;
	while (i < g_game_info.player_num)
	{
		close(g_players[i].soc);
		i++;
	}
}
