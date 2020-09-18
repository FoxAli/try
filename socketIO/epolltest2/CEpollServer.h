#ifndef EPOLL_SERVER_H
#define EPOLL_SERVER_H

#include <sys/socket.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string>

#define EVENT_COUNT 20
#define BUFFER_LENGTH  (1024 * 1024 * 6)
#define SOCKET int
#define LISTEN_NUM 50

using namespace std;

class CEpollServer
{
public:
	CEpollServer();
	~CEpollServer();

public:
	int Start(int port , string ip);
	int Stop();

private:
	int _port;
	string _ip;

	bool _run;
	
	char* _buffer;

	SOCKET _socket;
	
	epoll_event _ev;    //remove in the furture
	epoll_event _events[EVENT_COUNT];  //remove in the furture

	
	
};

#endif

