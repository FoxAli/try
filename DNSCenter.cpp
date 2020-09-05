
#include "stdHeader.h"
#include "DNSCenter.h"
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <unistd.h>
#include <event.h>
#include <evdns.h>
#include <arpa/inet.h>


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


	char* pTempDomain;
	
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
		free(last->Domain);
		free(last->HostIP);
		
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
	LOG(LOG_DEBUG , "DNS_CallBack in result:%d count:%d ", result , count);

	if(result != DNS_ERR_NONE || 0 == count)
	{
		event_loopexit(NULL); // not safe for multithreads
		return;
	}

	
	DNSCenter* center = (DNSCenter*)arg;
	pthread_mutex_lock(center->pMutex);

	if(NULL == center->First)
	{
		LOG(LOG_DEBUG , "DNS_CallBack first [%s][%s]"
		    , center->pTempDomain
		    , addresses);
		
		center->First = (DNSNode*)malloc(sizeof(DNSNode));
		memset(center->First , 0 , sizeof(DNSNode));

		center->First->Domain = strdup(center->pTempDomain);

		struct in_addr* addrs = (in_addr *)addresses;
		char* ip = inet_ntoa(addrs[0]);
		LOG(LOG_DEBUG , "ip:%s" , ip);
		center->First->HostIP = strdup(ip);
	}
	else
	{
		LOG(LOG_DEBUG , "DNS_CallBack tail");
		
		DNSNode* pLast = center->First;
		while(NULL != pLast->Next)
		{
			pLast = pLast->Next;
		}

		pLast->Next = (DNSNode*)malloc(sizeof(DNSNode));
		memset(pLast->Next , 0 , sizeof(DNSNode));

		pLast->Next->Domain = strdup(center->pTempDomain);
		
		struct in_addr* addrs = (in_addr *)addresses;
		char* ip = inet_ntoa(addrs[0]);
		LOG(LOG_DEBUG , "ip:%s" , ip);
		pLast->Next->HostIP = strdup(ip);
	}
	
	pthread_mutex_unlock(center->pMutex);

	event_loopexit(NULL); // not safe for multithreads
	LOG(LOG_DEBUG , "DNS_CallBack out");
}

char* ResolveDomainName (DNSCenter* center , char* domainName)
{
	bool once = false;
	while(1)
	{
		LOG(LOG_DEBUG , "ResolveDomainName while(1)");
		
		pthread_mutex_lock(center->pMutex);
		char* existIP = FindHostIP(center , domainName);
		if(NULL != existIP)
		{
			return existIP;
		}
		pthread_mutex_unlock(center->pMutex);

		if(!once)
		{
			LOG(LOG_DEBUG , "ResolveDomainName once");
			
			//once = true; 

			center->pTempDomain = domainName; //save for call back function
			
			event_base * base = event_init();
			evdns_init();
			evdns_resolve_ipv4(domainName, 0, DNS_CallBack, center);
			event_dispatch();
			event_base_free(base);

			LOG(LOG_DEBUG , "ResolveDomainName once is finished.");
		}

		sleep(1);
		
	}
	
	return NULL;
}






