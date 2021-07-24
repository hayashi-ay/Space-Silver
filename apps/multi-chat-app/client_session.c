#include "client_session.h"

int		g_soc;
int		g_nfds;
fd_set	g_mask;
char	g_send_buf[BUF_LEN];
char	g_recv_buf[BUF_LEN];
WINDOW	*g_win_send;
WINDOW	*g_win_recv;
WINDOW	*g_frame_send;
WINDOW	*g_frame_recv;

void	finalize_client_session(int	signal);

void	init_client_session(int	soc)
{
	g_soc = soc;
	g_nfds = soc + 1;
	FD_ZERO(&g_mask);
	FD_SET(0, &g_mask);
	FD_SET(soc, &g_mask);

	setup_client_window();
}

void	setup_client_window(void)
{
	initscr();
	signal(SIGINT, finalize_client_session);

	// 送信用ウィンドウ
	g_win_send = newwin(SEND_WIN_HEIGHT, SEND_WIN_WIDTH, 19, 1);
	scrollok(g_win_send, TRUE);
	wmove(g_win_send, 0, 0);
	g_frame_send = newwin(SEND_WIN_HEIGHT + 2, SEND_WIN_WIDTH + 2, 18, 0);
	box(g_frame_send, '|', '-');

	// 受信用ウィンドウ
	g_win_recv = newwin(RECV_WIN_HEIGHT, RECV_WIN_WIDTH, 1, 1);
	scrollok(g_win_recv, TRUE);
	wmove(g_win_recv, 0, 0);
	g_frame_recv = newwin(RECV_WIN_HEIGHT + 2, RECV_WIN_WIDTH + 2, 0, 0);
	box(g_frame_recv, '|', '-');

	cbreak();
	noecho();

	wrefresh(g_win_send);
	wrefresh(g_frame_send);
	wrefresh(g_win_recv);
	wrefresh(g_frame_recv);
}

void	loop_client_session(void)
{
	int		c;
	int		input_len;
	int		bytes;
	int		i;
	int		x;
	int		y;
	fd_set	copy_mask;

	input_len = 0;
	while (1)
	{

		copy_mask = g_mask;
		select(g_nfds, &copy_mask, NULL, NULL, NULL);
		if (FD_ISSET(0, &copy_mask))
		{
			c = getchar();
			if (c == '\b' || c == 0x7f)
			{
				if (input_len == 0)
					continue ;
				input_len--;
				getyx(g_win_send, y, x);
				wmove(g_win_send, y, x - 1);
				waddch(g_win_send, ' ');
				wmove(g_win_send, y, x - 1);
			}
			else if (c == '\n' || c == '\r')
			{
				g_send_buf[input_len] = '\n';
				input_len++;
				write(g_soc, g_send_buf, input_len);
				input_len = 0;
				wclear(g_win_send);
			}
			else
			{
				g_send_buf[input_len] = c;
				input_len++;
				waddch(g_win_send, c);
			}
			wrefresh(g_win_send);
		}

		if (FD_ISSET(g_soc, &copy_mask))
		{
			bytes = read(g_soc, g_recv_buf, BUF_LEN);
			i = 0;
			while (i < bytes)
			{
				waddch(g_win_recv, g_recv_buf[i]);
				i++;
			}

			if (strstr(g_recv_buf, "quit") != NULL)
			{
				break ;
			}
			wrefresh(g_win_recv);
		}
	}
	finalize_client_session(0);
}

void	finalize_client_session(int	signal)
{
	(void)signal;
	endwin();
	close(g_soc);
}
