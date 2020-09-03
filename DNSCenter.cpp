
#include "DNSCenter.h"
#include <stdlib.h>
#include <string.h>




struct DNSNode
{
	DNSNode* Next;
	DNSNode* Prior;

	char* Domain;
	char* hostIP;
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
		pNode == pNode->Prior;
		free(last);
		last = NULL;
	}
	
	free(center);
}

void ResolveDomainName (DNSCenter* center)
{
	
	
}




