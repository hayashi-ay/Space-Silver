#include "client_session.h"
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	const int	port = 9999;
	int			soc;
	char		*hostname = "localhost";
	
	soc = setup_client(port, hostname);
	if (soc == -1)
	{
		exit(1);
	}
	fprintf(stdout, "Successfully Connected!\n");

	init_client_session(soc);
	loop_client_session();
}
