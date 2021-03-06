#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "stdHeader.h"
#include "DNSCenter.h"
#include "ctype.h"
#include "Downloader.h"
#include "ParserResponse.h"

#define SAFE_FREE(p) \
	if(p)free(p); \
	p = NULL;

int IsInt(char* p)
{
	int ret = 0;
	while(0 != *p)   // 0 is the tail of c string 
	{
		if(*p < '0' || *p > '9')
		{
			ret = -1;
			break;
		}

		p++;
	}

	return ret;
}

int ParseDomainPort(char* pdomainport , Url* pUrl)
{
	int ret = 0;
	
	char* pSemicolon = strchr(pdomainport, ':');
	if(NULL == pSemicolon)
	{
		pUrl->Domain = strdup(pdomainport);
		pUrl->Port = 80;
		LOG(LOG_DEBUG , "Url is only domain %s" , pUrl->Domain);
	}
	else
	{
		int len = pSemicolon - pdomainport;
		char* domain = (char*)malloc(len + 1);
		memset(domain , 0 , len + 1);
		strncpy(domain , pdomainport , len);
		pUrl->Domain = domain;

		char* portStr = pSemicolon + 1;
		if(0 == IsInt(portStr))
		{
			int port = atoi(portStr);
			if(port > 0 && port <= 65535)
			{
				pUrl->Port = port;
				LOG(LOG_DEBUG , "Url is only domain %s and port is %d." , pUrl->Domain , pUrl->Port);
			}
			else
			{
				ret = -1;
				LOG(LOG_DEBUG , "Url port %d is invalid." , port);
			}
		}
		else
		{
			LOG(LOG_DEBUG , "Url port is not digit");
			ret = -1;
		}
			
	}

	return ret;
}


int ParseUrlString (Url* pUrl , char* urlString)
{
	//tram blank , space ~~~~ in the furture.
	
	LOG(LOG_DEBUG , "url string:%s" , urlString);

	char* pUrlString = urlString;
	if(strlen(pUrlString) < 9)   // 8 is the length of "https://" + 1 
	{
		LOG(LOG_DEBUG , "Url string is too short.");
		return -1;
	}

        if(0 != strncmp(pUrlString , "http://" , 7) &&
	   0 != strncmp(pUrlString , "https://" , 8))
	{
		LOG(LOG_DEBUG , "Protocol is not  http(s).");
		return -1;
	}

	int protocolLength = 0;
	if('s' == pUrlString[4])
	{
		pUrl->Protocol = strdup("https");
		LOG(LOG_DEBUG , "Protocol is %s" ,  pUrl->Protocol);
		
		protocolLength = 8;
		int remainingLength = strlen(pUrlString) - protocolLength;
		LOG(LOG_DEBUG , "Protocol is https and remain length is %d" , remainingLength);
	}
	else
	{
		pUrl->Protocol = strdup("http");
		LOG(LOG_DEBUG , "Protocol is %s" ,  pUrl->Protocol);
		
		protocolLength = 7;
		int remainingLength = strlen(pUrlString) - protocolLength;

		LOG(LOG_DEBUG , "Protocol is http and remain length is %d" , remainingLength);
	}

	char* pRemainString = pUrlString + protocolLength;
	LOG(LOG_DEBUG , "Remain string is (%s)" , pRemainString);

	char* urlServerTail = strchr(pRemainString , '/'); 
	char* urlServer = NULL;
	if(NULL == urlServerTail)
	{
		int length = strlen(pRemainString);
		LOG(LOG_DEBUG , "Url server length is %d" , length);
		urlServer = strndup(pRemainString , length);
		LOG(LOG_DEBUG , "server url is %s" , urlServer);
	}
	else
	{
		int length = urlServerTail - pRemainString;
		LOG(LOG_DEBUG , "Url server length (without directory) is %d" , length);
		urlServer = strndup(pRemainString , length);
		LOG(LOG_DEBUG , "server url is %s" , urlServer);
		pUrl->Directory = strdup(pRemainString + length); 

		LOG(LOG_DEBUG , "Directory is %s" , pUrl->Directory);
	}

	int ret = 0;
	
	//find @
	char* pAt = strchr(urlServer , '@');
	if(NULL == pAt)
	{
		ret = ParseDomainPort(urlServer , pUrl); 
	}
	else
	{
		//igore user and password ~~~~~
		LOG(LOG_DEBUG , "Igore user and password");
		
		char* pDomainPort = pAt + 1;
		ret = ParseDomainPort(pDomainPort , pUrl);

	}

	
	SAFE_FREE(urlServer);
	
	return ret;
}

void ReleaseUrl(Url* pUrl)
{
	if(!pUrl)return;
	
	SAFE_FREE(pUrl->Protocol);
	SAFE_FREE(pUrl->User);
	SAFE_FREE(pUrl->Password);
	SAFE_FREE(pUrl->Domain);
	SAFE_FREE(pUrl->IP);
	SAFE_FREE(pUrl->Directory);

	SAFE_FREE(pUrl);
}



int main(int argc,char **argv)
{
	char* seed = "https://www.163.com/";
	//char* seed = "https://www.163.com";
	//char* seed = "https://tech.163.com/20/0907/09/FLTOLBIO00097U7T.html";
	//char* seed = "https://wp:123456@tech.163.com:80/20/0907/09/FLTOLBIO00097U7T.html";
	//char* seed = "https://tech.163.com/20/0907/09/FLTOLBIO00097U7T.html";
	Url* pUrl = (Url*)malloc(sizeof(Url));
	int parseUrlRet = ParseUrlString(pUrl , seed);

	//ReleaseUrl(pUrl);
	
	//return 0;
	
	//char path[256] = {0};
	//char* pPath = getcwd(path , 256);
	//strcat(pPath , "/download");
	//printf("filePath: %s\n", pPath);
	
	DNSCenter* pDNSCenter = CreateDNSCenter();

	char* hostIP = ResolveDomainName(pDNSCenter , pUrl->Domain);
	pUrl->IP = strdup(hostIP);
	
	ReleaseDNSCenter(pDNSCenter);


	DownLoad(pUrl);




	ReleaseUrl(pUrl);
	
		
	return 0;
}

