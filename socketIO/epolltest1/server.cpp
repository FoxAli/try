#include <sys/socket.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define PORT 60000
#define LISTENQ 20

int main(int argc, char **argv)
{
	struct epoll_event ev,events[20];
	int epfd = epoll_create(256);

	struct sockaddr_in serveraddr;
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);


	ev.data.fd=listenfd;
	ev.events=EPOLLIN|EPOLLET;

	epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);

	memset(&serveraddr , 0,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	char *local_addr="127.0.0.1";
	inet_aton(local_addr,&(serveraddr.sin_addr));
	serveraddr.sin_port=htons(PORT);
	bind(listenfd,(sockaddr *)&serveraddr, sizeof(serveraddr));
	listen(listenfd, LISTENQ);

	for(;;)
	{
		int nfds = epoll_wait(epfd,events,20,500);
		printf("epoll_wait return %d" , nfds);
		
		for(int i=0;i<nfds;++i)
		{
			if(events[i].data.fd==listenfd)
			{
				socklen_t clilen = {0};
				struct sockaddr_in clientaddr = {0};
				int connfd = accept(listenfd,(sockaddr *)&clientaddr, &clilen);
				if(connfd < 0)
				{
					printf("connfd < 0");
					exit(1);
				}

				char* str = inet_ntoa(clientaddr.sin_addr);
				printf("accapt a connection from  %s" , str);

				ev.data.fd=connfd;
				ev.events=EPOLLIN|EPOLLET;
				epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
				
			}
			else if(events[i].events&EPOLLIN)
			{
				
			}
			else if(events[i].events&EPOLLOUT)
			{
				
			}
		}
	}
	
	return 0;
}



