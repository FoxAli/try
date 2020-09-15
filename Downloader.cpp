#include "Downloader.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "stdHeader.h" 
#include "ParserResponse.h"


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

int SendRequest(int* sock, Url* pUrl)
{
	char request[1024] = {0};
	
	sprintf(request, "GET /%s HTTP/1.0\r\n"
		"Host: %s\r\n"
		"Accept: */*\r\n"
		"Connection: Keep-Alive\r\n"
		"User-Agent: Mozilla/5.0 (compatible; Qteqpidspider/1.0;)\r\n"
		"Referer: %s\r\n\r\n", pUrl->Directory, pUrl->Domain, pUrl->Domain);

	int need, begin, n;
	need = strlen(request);
	begin = 0;
	while (need) 
	{
		n = write(*sock, request + begin, need);
		if (n <= 0) 
		{
			if (errno == EAGAIN) 
			{ 
				//write buffer full, delay retry
				usleep(1000);
				continue;
			}
			LOG(LOG_DEBUG, "Fail to send request.");
			close(*sock);
			return -1;
		}
		begin += n;
		need -= n;
	}

	LOG(LOG_DEBUG, "Success to send request.");

	return 0;
}




int ReceiveResponse(int* sock , char* responseBody)
{
	int len = 0;
	while (1)
	{
		int n = read(*sock, responseBody + len, 1024);
		if (n < 0)
		{
			LOG(LOG_DEBUG, "Fail to receive response.");
			return -1;
		}
		else if (0 == n)
		{
			LOG(LOG_DEBUG, "Success to receive response. length: %d " , len);

			FILE* fp = fopen("test.txt", "wb+");
			fwrite(responseBody, sizeof(char), HTML_MAXLEN, fp);
			fclose(fp);

			break;
		}
		else
		{
			
			len += + n;
			LOG(LOG_DEBUG, "Receive respnse length %d , total length %d ", n, len);
		}
	}

	return 0;
}


void DownLoad(Url* pUrl)
{
	int sock = 0;
	if(0 != Connect(&sock, pUrl->IP , pUrl->Port))
	{
		return;
	}

	if (0 != SendRequest(&sock, pUrl))
	{
		return;
	}
	
	char* responseBody = new char[HTML_MAXLEN];
	memset(responseBody, 0, HTML_MAXLEN * sizeof(char));
	if (0 == ReceiveResponse(&sock , responseBody))
	{
		int findPos;
		EHyperlinkType hyperlinkType;
		ParserString(responseBody, &findPos, &hyperlinkType);
	}

	delete responseBody;
	responseBody = NULL;

}




