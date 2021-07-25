#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "race.h"
#include "client_session.h"
#include "utils.h"

int		g_player_num;
int		g_stage_num;
int		g_soc;
int		g_entry_num;

void	finalize_client_session(int	signal);
void	init_data(void);

void	init_client_session(int	soc)
{
	char	buf[BUF_LEN];

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

	init_data();
	return ;
}

void	loop_client_session(void)
{
	return ;
}

void	init_data(void)
{

}
