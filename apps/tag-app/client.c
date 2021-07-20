#include <stdlib.h>
#include "tag.h"

int	main(void)
{
	int		soc;
	char	*hostname = "localhost";

	soc = setup_client(PORT, hostname);
	if (soc == -1)
		exit(1);
	init_tag(soc, 'x', 10, 10, 'o', 1, 1);
	loop_tag();
}
