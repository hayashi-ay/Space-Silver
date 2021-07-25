#include <stdlib.h>
#include "race.h"
#include "utils.h"
#include "client_session.h"

int	main(void)
{
	int		soc;
	char	*hostname = "localhost";

	soc = setup_client(PORT, hostname);
	if (soc == -1)
	{
		exit(1);
	}
	init_client_session(soc);
	loop_client_session();
}
