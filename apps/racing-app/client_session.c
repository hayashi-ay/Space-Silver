#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>
#include "race.h"
#include "client_session.h"
#include "utils.h"

int		g_player_num;
int		g_stage_num;
int		g_soc;
int		g_entry_num;
char	g_send_data[PLAYER_INFO_SIZE];
char	g_players_info[MAX_PLAYER*PLAYER_INFO_SIZE];
WINDOW	*g_win_main;
WINDOW	*g_win_sub;

void	finalize_client_session(int	signal);
void	init_data(void);
int		get_position(void);
void	send_data(void);
void	recv_data(void);
void	show_players(void);
int		load_stage(int	num);
int		check(int	xd, int	yd);

void	init_client_session(int	soc)
{
	char	buf[BUF_LEN];
	char	*me;

	g_soc = soc;
	// ログイン処理
	read(g_soc, buf, BUF_LEN);
	printf("%s", buf);
	fgets(buf, BUF_LEN, stdin);
	chop_newline(buf);
	write(g_soc, buf, strlen(buf));
	// エントリーナンバーの受け取り
	read(g_soc, buf, BUF_LEN);
	sscanf(buf, "%d", &g_entry_num);
	printf("Your entry number is %d\n", g_entry_num);
	// ゲームの情報の受け取り
	read(g_soc, buf, BUF_LEN);
	sscanf(buf, "%d %d", &g_player_num, &g_stage_num);
	printf("Players are %d\n", g_player_num);
	printf("Stages are %d\n", g_stage_num);

	// 初期データの受取
	recv_data();
	me = g_players_info + PLAYER_INFO_SIZE * g_entry_num;
	memcpy(g_send_data, me, PLAYER_INFO_SIZE);

	// 表示処理
	signal(SIGINT, finalize_client_session);

	initscr();
	noecho();

	g_win_main = newwin(22, 12, 1, 10);
	g_win_sub = newwin(11, 25, 5, 40);
	box(g_win_main, '|', '-');
	box(g_win_sub, '|', '-');

	load_stage(1);
	show_players();
}

void	loop_client_session(void)
{
	int	flag;

	flag = 1;
	while (flag)
	{
		flag = get_position();
		send_data();
		recv_data();
		show_players();
	}
	finalize_client_session(0);
}

int	get_position(void)
{
	char	c;

	read(0, &c, 1);
	if (c == KEYMAP_UP)
	{
		if (g_send_data[OFFSET_Y] == (char)1)
		{
			g_send_data[OFFSET_STAGE]++;
			g_send_data[OFFSET_Y] = (char)21;
			if (load_stage((int)g_send_data[OFFSET_STAGE]) == 0)
				return (0);
		}
		else if (check(0, 1))
		{
			g_send_data[OFFSET_Y]--;
		}
		else
			g_send_data[OFFSET_DAMAGE]++;
	}
	else if (c == KEYMAP_DOWN)
	{
		if (check(0, 1))
			g_send_data[OFFSET_Y]++;
		else
			g_send_data[OFFSET_DAMAGE]++;
	}
	else if (c == KEYMAP_RIGHT)
	{
		if (check(0, 1))
			g_send_data[OFFSET_X]++;
		else
			g_send_data[OFFSET_DAMAGE]++;
	}
	else if (c == KEYMAP_LEFT)
	{
		if (check(0, -1))
			g_send_data[OFFSET_X]--;
		else
			g_send_data[OFFSET_DAMAGE]++;
	}
	if (g_send_data[OFFSET_DAMAGE] >= MAX_DAMAGE)
	{
		return (0);
	}
	return (1);
}

void	send_data(void)
{
	write(g_soc, &send_data, PLAYER_INFO_SIZE);
}

void	recv_data(void)
{
	read(g_soc, &g_players_info, MAX_PLAYER * PLAYER_INFO_SIZE);

	int	i;
	i = 0;
	while (i < 4)
	{
		printf("%d\n", g_players_info[i]);
		fflush(stdout);
		i++;
	}
}

int	load_stage(int	num)
{
	FILE	*fp;
	int		i;
	char	file_name[16];
	char	buf[STAGE_WIDTH + 1];

	if (num > g_stage_num)
	{
		return (0);
	}
	sprintf(file_name, "data/data%d", num);
	fp = fopen(file_name, "r");
	i = 1;
	while (i < STAGE_LENGTH)
	{
		fgets(buf, 11, fp);
		buf[10] = '\0';
		wmove(g_win_main, i, 1);
		waddstr(g_win_main, buf);
		i++;
	}
	fclose(fp);
	return (0);
}

int	check(int	xd, int	yd)
{
	(void)xd;
	(void)yd;
	return (1);
}

void	show_players(void)
{
	wrefresh(g_win_main);
}

void	finalize_client_session(int	signal)
{
	(void)signal;
	endwin();
}
