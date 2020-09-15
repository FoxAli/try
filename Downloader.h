#ifndef DOWNLOADER_H
#define DOWNLOADER_H

//#include "stdHeader.h"

//http(s)://username:password@api.somesite.com/test/blah?something=123
struct Url
{
	char* Protocol; //"http" or "https"
	char* User;
	char* Password;
	char* Domain;
	char* IP;
	int Port;
	char* Directory;
};

void DownLoad(Url* pUrl);

#define HTML_MAXLEN   1024*1024

#endif
