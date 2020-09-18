#include <stdio.h>

#include "CEpollServer.h"

int main(int argc, char *argv[])
{
	printf("server main function in. \n");
	
	const int port = 6000;
	const string ip = "127.0.0.1";

	
	CEpollServer server;
	server.Start(port , ip);

	int a = getchar();
	
	server.Stop();
	
	return 0;
}

