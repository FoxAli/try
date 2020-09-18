
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<unistd.h>
#include <fcntl.h>
#include<unistd.h>


#define SOCKET int

int main(int argc, char *argv[])
{
	char buffer[1000 * 1000 * 2];
	for(int i = 0 ; i < 10 ; ++i)
	{
		buffer[i] = i;
	}

	
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(6000);
	inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

	if (fcntl(sock, F_SETFL, fcntl(sock, F_GETFD, 0)|(~O_NONBLOCK)) == -1)
	{
		printf("fail to set socket to no block mode.");
	}
	
	int r = connect(sock, (struct sockaddr*)&servaddr , sizeof(servaddr));
	if(r < 0)
	{
		printf("fail to connect to server : %d \n" , r);
	}
	else
	{
		printf("seccess to connect to server: %d \n", r );
	}

	sleep(2);
	for(int i = 0 ; i < 20 ; ++i)
	{
		int ss = send(sock, buffer , 10 , 0);
		printf("send return: %d \n" , ss);

		if(ss < 0)
		{
			
		}
	}

	close(sock);
	
	return 0;
}


