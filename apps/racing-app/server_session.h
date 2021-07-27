#ifndef SERVER_SESSION_H
# define SERVER_SESSION_H
# include <stdio.h>
# include <string.h>
# include <sys/select.h>
# include <unistd.h>
# include "race.h"
void	entry(int	i, int	fd);
void	init_server_session(int	player_num, int	stage_num, int	max_fd);
void	loop_server_session(void);
#endif
