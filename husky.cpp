#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

#include "stdHeader.h"
#include "DNSCenter.h"


//http://username:password@api.somesite.com/test/blah?something=123
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

Url* ParseUrlString ()
{
	
}

void ReleaseUrl(Url* pUrl)
{
	SAFE_FREE(pUrl->Protocol);
	SAFE_FREE(pUrl->User);
	SAFE_FREE(pUrl->Password);
	SAFE_FREE(pUrl->IP);
	SAFE_FREE(pUrl->Directory);
}



int main(int argc,char **argv)
{
	//char* seed = "https://www.163.com/";
	//char* seed = "www.163.com";
	char* seed = "https://tech.163.com/20/0907/09/FLTOLBIO00097U7T.html";
	
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

