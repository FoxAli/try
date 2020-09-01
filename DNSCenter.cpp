
#include "DNSCenter.h"
#include <stdlib.h>
#include <string.h>

struct DNSNode
{
	DNSNode* Next;
	DNSNode* Prior;
};

struct DNSCenter
{
	DNSNode* node;
};



DNSCenter* CreateDNSCenter()
{
	DNSCenter* center = (DNSCenter*)malloc(sizeof(DNSCenter));
	memset(center , 0 , sizeof(DNSCenter));
}

void ReleaseDNSCenter(DNSCenter* center)
{
	
}

void ResolveDomainName (DNSCenter* center)
{
	
}




