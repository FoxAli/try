#include <sys/socket.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define PORT 60000
#define LISTENQ 20
#define MAXLINE 5

int main(int argc, char **argv)
{
	char line[MAXLINE];
	
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
		printf("epoll_wait return %d \n" , nfds);
		
		for(int i=0;i<nfds;++i)
		{
			if(events[i].data.fd==listenfd)
			{
				socklen_t clilen = {0};
				struct sockaddr_in clientaddr = {0};
				int connfd = accept(listenfd,(sockaddr *)&clientaddr, &clilen);
				if(connfd < 0)
				{
					printf("connfd < 0 \n");
					exit(1);
				}

				char* str = inet_ntoa(clientaddr.sin_addr);
				printf("accapt a connection from  %s \n" , str);

				ev.data.fd=connfd;
				ev.events=EPOLLIN|EPOLLET;
				epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
				
			}
			else if(events[i].events&EPOLLIN)
			{
				printf("EPOLLIN \n");
				int sockfd = events[i].data.fd;
				if(sockfd < 0)
				{
					printf("EPOLLIN but sockfd < 0 \n");
					continue;
				}

				int n = read(sockfd , line , MAXLINE);
				if(n < 0)
				{
					if (errno == ECONNRESET)
					{
						close(sockfd);
						events[i].data.fd = -1;
					}
					else
					{
						printf("readline error \n");
					}
				}
				else if(0 == n)
				{
					close(sockfd);
					events[i].data.fd = -1;
				}

				line[n] = '/0';
				printf("read: %s \n" , line);

				ev.data.fd=sockfd;
				ev.events = EPOLLOUT|EPOLLET;
					
			}
			else if(events[i].events&EPOLLOUT)
			{
				int sockfd = events[i].data.fd;
				int n = MAXLINE;
				write(sockfd, line, n);

				ev.data.fd=sockfd;
				ev.events=EPOLLIN|EPOLLET;
				epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
			}
		}
	}
	
	return 0;
}



