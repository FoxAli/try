
#include "stdHeader.h"
#include "DNSCenter.h"
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <unistd.h>
#include <event.h>
#include <evdns.h>



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
	pthread_mutex_t* pMutex;
};



DNSCenter* CreateDNSCenter()
{
	DNSCenter* center = (DNSCenter*)malloc(sizeof(DNSCenter));
	memset(center , 0 , sizeof(DNSCenter));
	
	center->pMutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	int code = pthread_mutex_init(center->pMutex, NULL);
	LOG(LOG_DEBUG , "CreateDNSCenter create mutex, code:%d", code);
	
	return center;
}

void ReleaseDNSCenter(DNSCenter* center)
{
	int code = pthread_mutex_destroy(center->pMutex);
	LOG(LOG_DEBUG , "CreateDNSCenter release mutex, code:%d", code);
	
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

void DNS_CallBack(int result, char type, int count, int ttl, void *addresses, void *arg)
{
	
}

char* ResolveDomainName (DNSCenter* center , char* domainName)
{
	bool once = false;
	while(1)
	{
		char* existIP = FindHostIP(center , domainName);
		if(NULL != existIP)
		{
			return existIP;
		}


		if(!once)
		{
			once = true; 
			
			event_base * base = event_init();
			evdns_init();
			evdns_resolve_ipv4(domainName, 0, DNS_CallBack, center);
			event_dispatch();
			event_base_free(base);
		}

		sleep(1);
		
	}
	
	return NULL;
}






