#ifndef GAMES_H
# define GAMES_H
# include "utils.h"
void	init_game(int	soc, char	peer_stone, char	my_stone);
void	show_game(void);
int		peer_turn(void);
int		my_turn(void);
void	finalize_game(void);
#endif
