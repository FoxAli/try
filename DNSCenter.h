#ifndef DNS_CENTER_H
#define DNS_CENTER_H


struct DNSCenter;

DNSCenter* CreateDNSCenter();
void ReleaseDNSCenter(DNSCenter* center);
void ResolveDomainName (DNSCenter* center);



#endif



