#ifndef STD_HEADER_H
#define STD_HEADER_H

#include <stdio.h>
#include <time.h>

#define MAX_LOG_SIZE 1024

#define DEBUG "debug"
#define INFO  "info"
#define WARN  "warn"
#define ERROR "error"
#define FATAL "fatal"



#define LOG(level, format, ...) \
	{ \
	printf("[file name: %s , function: %s , line: %d]",	\
	       __FILE__,__FUNCTION__,__LINE__);  \
	char formatlog[MAX_LOG_SIZE]; \
	sprintf(formatlog ,format , ##__VA_ARGS__);	\
	time_t now = time(NULL); \
	char buf[32]; \
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now)); \
	printf("[%s][%s]\n" ,buf , formatlog); \
	}



#endif

