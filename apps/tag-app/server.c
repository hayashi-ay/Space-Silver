#include <stdlib.h>
#include "tag.h"

int	main(void)
{
	int	soc;

	soc = setup_server(PORT);
	if (soc == -1)
		exit(1);
	init_tag(soc, 'o', 1, 1, 'x', 10, 10);
	loop_tag();
}
