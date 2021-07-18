#ifndef GAMES_H
# define GAMES_H
# include "utils.h"
void	init_game(int	soc, char	server_stone, char	client_stone);
void	show_game(void);
int		server_turn(void);
int		client_turn(void);
void	finalize_game(void);
#endif
