#include "tag.h"

typedef struct s_player {
	int		x;
	int		y;
	char	c;
}	t_player;

typedef struct s_keymaps
{
	int	left;
	int	down;
	int	up;
	int	right;
	int	quit;
}	t_keymaps;

int				g_soc;
fd_set			g_mask;
int				g_desc_num;
char			g_buf[20];
WINDOW			*g_win;
t_player		g_me;
t_player		g_peer;
const t_keymaps	g_keymaps = {'h', 'j', 'k', 'l', 'q'};

int		max(int	a, int	b);
int		min(int	a, int	b);
void	show(t_player	*p);
void	hide(t_player	*p);
int		update(t_player	*p, int	c);
int		interpret(t_player *p);
void	finalize(int	signal);

void	init_tag(int soc, char	mc,	int	mx, int	my, char	pc, char	px, char	py)
{
	g_soc = soc;
	FD_ZERO(&g_mask);
	FD_SET(0, &g_mask);
	FD_SET(g_soc, &g_mask);
	g_desc_num = g_soc + 1;

	g_me.c = mc;
	g_me.x = mx;
	g_me.y = my;
	g_peer.c = pc;
	g_peer.x = px;
	g_peer.y = py;

	initscr();
	signal(SIGINT, finalize);
	g_win = newwin(MAX_Y + 2, MAX_X + 2, 0, 0);
	box(g_win, '|', '-');

	cbreak();
	noecho();
}

void	loop_tag(void)
{
	fd_set	copy_mask;
	int		exist_key_input;
	int		exist_socket_input;
	int		c;
	int		flag;

	show(&g_me);
	show(&g_peer);
	
	flag = 1;
	while (flag)
	{
		copy_mask = g_mask;
		select(g_desc_num, &copy_mask, NULL, NULL, NULL);

		exist_key_input = FD_ISSET(0, &copy_mask);
		exist_socket_input = FD_ISSET(g_soc, &copy_mask);

		if (exist_key_input)
		{
			c = getchar();
			hide(&g_me);
			flag = update(&g_me, c);
			show(&g_me);
			write(g_soc, g_buf, 20);
		}

		if (exist_socket_input && flag)
		{
			read(g_soc, g_buf, 20);
			hide(&g_peer);
			flag = interpret(&g_peer);
			show(&g_peer);
		}
	}
	finalize(SIGQUIT);
}

int	max(int	a, int	b)
{
	if (a > b)
		return (a);
	return (b);
}

int	min(int	a, int	b)
{
	if (a < b)
		return (a);
	return (b);
}

void	show(t_player	*p)
{
	wmove(g_win, p->y, p->x);
	waddch(g_win, p->c);
	// カーソルの位置を戻して文字と重なるように
	wmove(g_win, p->y, p->x);
	wrefresh(g_win);
}

void	hide(t_player	*p)
{
	wmove(g_win, p->y, p->x);
	waddch(g_win, ' ');
}

int	update(t_player	*p, int	c)
{
	if (c == g_keymaps.up)
		p->y = max(p->y - 1, MIN_Y);
	else if (c == g_keymaps.down)
		p->y = min(p->y + 1, MAX_Y);
	else if (c == g_keymaps.right)
		p->x = min(p->x + 1, MAX_X);
	else if (c == g_keymaps.left)
		p->x = max(p->x - 1, MIN_X);
	else if (c == g_keymaps.quit)
	{
		g_buf[0] = g_keymaps.quit;
		return (0);
	}
		
	sprintf(g_buf, "%d %d", p->x, p->y);
	return (1);
}

int	interpret(t_player *p)
{
	if (g_buf[0] == g_keymaps.quit)
		return (0);
	sscanf(g_buf, "%d %d", &p->x, &p->y);
	return (1);
}

void	finalize(int	signal)
{
	(void)signal;
	endwin();
	close(g_soc);
}
