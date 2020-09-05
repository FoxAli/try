#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "stdHeader.h"
#include "DNSCenter.h"


int main(int argc,char **argv)
{
	//char* seed = "https://www.163.com/";
	char* seed = "www.163.com";
	
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

