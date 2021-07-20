#ifndef TAG_H
# define TAG_H
# include <curses.h>
# include <signal.h>
# include <stddef.h>
# include <sys/select.h>
# include "utils.h"
# define MIN_X 1
# define MIN_Y 1
# define MAX_X 20
# define MAX_Y 60
# define PORT 9999
void	init_tag(int soc, char	mc,	int	mx, int	my, char	pc, char	px, char	py);
void	loop_tag(void);
#endif
