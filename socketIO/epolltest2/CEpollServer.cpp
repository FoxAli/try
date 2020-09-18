
#include "CEpollServer.h"


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
	_run = true;
	
	_port = port;
	_ip = ip;
		
	_ev.data.fd = _socket;
	_ev.events = EPOLLIN|EPOLLET;
	int epfd = epoll_create(256);
	epoll_ctl(epfd,EPOLL_CTL_ADD,_socket,&_ev);
	
	_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serveraddr;
	memset(&serveraddr , 0,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_aton(_ip.c_str() ,&(serveraddr.sin_addr));
	serveraddr.sin_port=htons(_port);
	
	bind(_socket ,(sockaddr *)&serveraddr, sizeof(serveraddr));
	listen(_socket, LISTEN_NUM);

	while(_run)
	{
		int fdnum =  epoll_wait(epfd,_events,EVENT_COUNT,500);

		for(int i = 0 ; i < fdnum ; ++i)
		{
			if(_events[i].data.fd == _socket)   //this is accept event 
			{
				SOCKET client = 0;
				do
				{
					socklen_t clilen = {0};
					struct sockaddr_in clientaddr = {0};
					client = accept(_socket,(sockaddr *)&clientaddr, &clilen);
					printf("accept return value %d " , client);
					
					char* clientIP = inet_ntoa(clientaddr.sin_addr);
					printf("accept a connection from  %s \n" , clientIP);

					_ev.data.fd = client;
					_ev.events=EPOLLIN|EPOLLET;
					epoll_ctl(epfd,EPOLL_CTL_ADD,client,&_ev);
					
				}
				while(client > 0);
				
			}
			else if(_events[i].events & EPOLLIN)
			{
				
			}
			else if(_events[i].events & EPOLLOUT)
			{
				
			}
			
		}
		
	}
	
	return 0;
}

int CEpollServer::Stop()
{
	_run = false;
	
	return 0;
}


