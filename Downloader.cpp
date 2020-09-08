#include "Downloader.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include "stdHeader.h" 


int Connect(int* sock , char* ip , int port)
{
	LOG(LOG_DEBUG , "Begin to connect to web server.");
	
	sockaddr_in server_addr;
	memset(&server_addr , 0 , sizeof(sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	if (!inet_aton(ip, &(server_addr.sin_addr)))
	{
		LOG(LOG_DEBUG , "Fail to convert server ip.");
		return -1;
	}

	if ((*sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		LOG(LOG_DEBUG , "Fail to create response socket.");
		return -1;
	}

	if (connect(*sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)) < 0)
	{
		close(*sock);
		LOG(LOG_DEBUG , "Fail to connect to web server.");
		return -1;
	}

	LOG(LOG_DEBUG , "Success to connect to web server.");
	
	return 0;
}


void DownLoad(Url* pUrl)
{
	int respSocket = 0;
	Connect(&respSocket , pUrl->IP , pUrl->Port);
	
}




