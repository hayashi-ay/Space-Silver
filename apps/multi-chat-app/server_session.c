#include "server_session.h"

typedef struct s_attendant
{
	int		fd;
	char	name[16];
}	t_attendant;

int					g_attendant_num;
int					g_nfds;
fd_set				g_mask;
static t_attendant	g_attendants[MAX_ATTENDANT];
char				g_buf[BUF_LEN];

int		finalize_server_session(void);
void	send_all_client(int	attendant_index, int	buf_len);

void	init_attendant(int	i, int	fd)
{
	int	bytes;

	g_attendants[i].fd = fd;
	memset(g_attendants[i].name, 0, MAX_NAME_LEN);
	dprintf(fd, "server --> Please type your name.\n");
	bytes = read(fd, g_attendants[i].name, MAX_NAME_LEN);
	// 改行を取り除く
	g_attendants[i].name[bytes - 1] = '\0';
	dprintf(fd, "server --> Please wait.\n");
}

void	init_server_session(int	attendant_num, int	max_fd)
{
	int	i;

	fprintf(stdout, "Initialize server session.\n");

	g_attendant_num = attendant_num;
	g_nfds = max_fd + 1;

	FD_ZERO(&g_mask);
	FD_SET(0, &g_mask);
	i = 0;
	while (i < attendant_num)
	{
		FD_SET(g_attendants[i].fd, &g_mask);
		i++;
	}
	i = 0;
	while (i < attendant_num)
	{
		dprintf(g_attendants[i].fd, "Communication is ready.\n");
		i++;
	}
}

void	loop_server_session(void)
{
	fd_set	copy_mask;
	int		i;
	int		flag;
	int		is_set;
	int		bytes;

	flag = 1;
	while (flag)
	{
		copy_mask = g_mask;
		select(g_nfds, &copy_mask, NULL, NULL, NULL);
		if (FD_ISSET(0, &copy_mask))
			flag = finalize_server_session();
		i = 0;
		while (flag && i < g_attendant_num)
		{
			is_set = FD_ISSET(g_attendants[i].fd, &copy_mask);
			if (is_set)
			{
				bytes = read(g_attendants[i].fd, g_buf, BUF_LEN);
				send_all_client(i, bytes);
			}
			i++;
		}
	}
}

int	finalize_server_session(void)
{
	int	i;

	i = 0;
	while (i < g_attendant_num)
	{
		dprintf(g_attendants[i].fd, "quit");
		i++;
	}
	i = 0;
	while (i < g_attendant_num)
	{
		close(g_attendants[i].fd);
		i++;
	}
	return (0);
}

void	send_all_client(int	attendant_index, int	buf_len)
{
	int	i;

	i = 0;
	while (i < g_attendant_num)
	{
		(void)attendant_index;
		write(g_attendants[i].fd, g_attendants[attendant_index].name, strlen(g_attendants[attendant_index].name));
		write(g_attendants[i].fd, " --> ", 5);
		write(g_attendants[i].fd, g_buf, buf_len);
		i++;
	}
}
