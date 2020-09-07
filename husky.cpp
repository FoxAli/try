#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

#include "stdHeader.h"
#include "DNSCenter.h"


//http(s)://username:password@api.somesite.com/test/blah?something=123
struct Url
{
	char* Protocol; //"http" or "https"
	char* User;
	char* Password;
	char* IP;
	int Port;
	char* Directory;
};

#define SAFE_FREE(p) \
	if(p)free(p); \
	p = NULL;

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
		char* protocol = (char*)malloc(5);
		strcpy(protocol , "http");
		pUrl->Protocol = protocol;

		protocolLength = 8;
		int remainingLength = strlen(pUrlString) - protocolLength;
		LOG(LOG_DEBUG , "Protocol is https and remain length is %d" , remainingLength);
	}
	else
	{
		char* protocol = (char*)malloc(4);
		strcpy(protocol , "https");
		pUrl->Protocol = protocol;

		protocolLength = 7;
		int remainingLength = strlen(pUrlString) - protocolLength;

		LOG(LOG_DEBUG , "Protocol is http and remain length is %d" , remainingLength);
	}

	char* pRemainString = pUrlString + protocolLength;
	LOG(LOG_DEBUG , "Remain string is (%s)" , pRemainString);
	
	
	return 0;
}

void ReleaseUrl(Url* pUrl)
{
	if(!pUrl)return;
	
	SAFE_FREE(pUrl->Protocol);
	SAFE_FREE(pUrl->User);
	SAFE_FREE(pUrl->Password);
	SAFE_FREE(pUrl->IP);
	SAFE_FREE(pUrl->Directory);

	SAFE_FREE(pUrl);
}



int main(int argc,char **argv)
{
	//char* seed = "https://www.163.com/";
	//char* seed = "www.163.com";
	char* seed = "https://tech.163.com/20/0907/09/FLTOLBIO00097U7T.html";
	Url* pUrl = (Url*)malloc(sizeof(Url));
	int parseUrlRet = ParseUrlString(pUrl , seed);
	SAFE_FREE(pUrl);

	return 0;
	
	char path[256] = {0};
	char* pPath = getcwd(path , 256);
	strcat(pPath , "/download");
	printf("filePath: %s\n", pPath);
	
	DNSCenter* pDNSCenter = CreateDNSCenter();

	char* hostIP = ResolveDomainName(pDNSCenter , seed);
	
	ReleaseDNSCenter(pDNSCenter);

	LOG(LOG_DEBUG , "a(%s)a" , "~~");
	LOG(LOG_INFO , "BB")
	
	return 0;
}

