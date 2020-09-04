
#include "DNSCenter.h"
#include <stdlib.h>
#include <string.h>




struct DNSNode
{
	DNSNode* Next;
	DNSNode* Prior;

	char* Domain;
	char* HostIP;
};

struct DNSCenter
{
	DNSNode* First;
};



DNSCenter* CreateDNSCenter()
{
	DNSCenter* center = (DNSCenter*)malloc(sizeof(DNSCenter));
	memset(center , 0 , sizeof(DNSCenter));

	return center;
}

void ReleaseDNSCenter(DNSCenter* center)
{
	DNSNode* pNode = center->First;
	if(pNode != NULL)
	{
		while(NULL != pNode->Next)
		{
			pNode = pNode->Next;
		}
	}

	while(pNode != NULL)
	{
		DNSNode* last = pNode;
		pNode = pNode->Prior;
		free(last);
		last = NULL;
	}
	
	free(center);
}


static char* FindHostIP(DNSCenter* center ,char* domain)
{
	DNSNode* node = center->First;
	while(NULL != node)
	{
		if(0 == strcmp(node->Domain , domain))
		{
			return node->HostIP;
		}
		
		node = node->Next;
	}

	return NULL;
}

char* ResolveDomainName (DNSCenter* center , char* domainName)
{
	char* existIP = FindHostIP(center , domainName);
	if(NULL != existIP)
	{
		return existIP;
	}

	
		
	return NULL;
}




