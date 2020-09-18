
#include "CEpollServer.h"
#include<unistd.h>
#include <fcntl.h>
#include <error.h>

CEpollServer::CEpollServer()
{
	_buffer = new char[BUFFER_LENGTH];
	memset(_buffer , 0 , BUFFER_LENGTH * sizeof(char));

	memset(&_ev , 0 , sizeof(epoll_event));
	memset(_events , 0 , EVENT_COUNT * sizeof(epoll_event));
}

CEpollServer::~CEpollServer()
{
	delete _buffer;
	_buffer = NULL;
}

int CEpollServer::Start(int port , string ip)
{
	printf("server is starting port %d ,ip %s  \n",port , ip.c_str());
	_run = true;
	
	_port = port;
	_ip = ip;
		
	_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (fcntl(_socket, F_SETFL, fcntl(_socket, F_GETFD, 0)|O_NONBLOCK) == -1)
	{
		printf("fcntl set socket nonblock failed . ");
	}
	
	struct sockaddr_in serveraddr;
	memset(&serveraddr , 0,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_aton(_ip.c_str() ,&(serveraddr.sin_addr));
	serveraddr.sin_port=htons(_port);
	
	bind(_socket ,(sockaddr *)&serveraddr, sizeof(serveraddr));
	printf("server is prepareing for listen. \n");
	listen(_socket, LISTEN_NUM);
	printf("server is listening. \n");

	_ev.data.fd = _socket;
	_ev.events = EPOLLIN|EPOLLET;
	int epfd = epoll_create(256);
	epoll_ctl(epfd,EPOLL_CTL_ADD,_socket,&_ev);
	
	while(_run)
	{
		int fdnum =  epoll_wait(epfd,_events,EVENT_COUNT,2000);
		printf("epoll_wait return %d \n" , fdnum);

		
		for(int i = 0 ; i < fdnum ; ++i)
		{
			if(_events[i].data.fd == _socket)   //this is accept event 
			{
				printf("epoll is start for accept; \n");
				
				SOCKET client = 0;
				do
				{
					socklen_t clilen = {0};
					struct sockaddr_in clientaddr = {0};
					client = accept(_socket,(sockaddr *)&clientaddr, &clilen);
					printf("accept return value %d \n" , client);
					
					char* clientIP = inet_ntoa(clientaddr.sin_addr);
					printf("accept a connection from  %s \n" , clientIP);

					if(clientIP > 0)
					{
						_ev.data.fd = client;
						_ev.events=EPOLLIN|EPOLLET;
						epoll_ctl(epfd,EPOLL_CTL_ADD,client,&_ev);
					
					}
				}
				while(client > 0);
				
			}
			else if(_events[i].events & EPOLLIN)
			{
				printf("EPOLLIN \n");
				SOCKET client = _events[i].data.fd;
				int n = read(client , _buffer , BUFFER_LENGTH);
				if(n < 0)
				{
					if (errno == ECONNRESET)
					{
						close(client);
						_events[i].data.fd = -1;
					}
					else
					{
						printf("readline error \n");
					}
				}

				_buffer[n] = '/0';
				printf("receive(%d):%s \n" , n, _buffer);
				
			}
			else if(_events[i].events & EPOLLOUT)
			{
				
			}
			
		}
		
	}

	close(epfd);
	
	return 0;
}

int CEpollServer::Stop()
{
	_run = false;
	
	return 0;
}


