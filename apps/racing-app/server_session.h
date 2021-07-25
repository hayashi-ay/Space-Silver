#ifndef SERVER_SESSION_H
# define SERVER_SESSION_H
void	entry(int	i, int	fd);
void	init_server_session(int	player_num, int	stage_num, int	max_fd);
void	loop_server_session(void);
#endif
