
#include "ParserResponse.h"
#include <regex.h>
#include<string.h>
#include "stdHeader.h"

//#define REGEX_HYPERLINK "href=\"(https?:)?\\\\([A-Z]|[a-z]|.|?|&|[0-9]|=)+\""
#define REGEX_HYPERLINK "href=\"https?:\\\\([A-Z]|[a-z]|.|?|&|[0-9]|=)+\""

//find return 0 , not find return -1
int ParserString(char* pString , int* findPos , EHyperlinkType* hyperlinkType)
{
	char* p = strstr(pString , "href=");
	int pos = p - pString;
	LOG(LOG_DEBUG, "strstr find pos %d ", pos);


	regex_t re;

	if (regcomp(&re, REGEX_HYPERLINK, REG_NEWLINE| REG_NOSUB) != 0)
	{
		LOG(LOG_DEBUG, "Fail to compile regex.");
		return -2;
	}

	regmatch_t matchArray[1];
	if (regexec(&re, pString, 1, matchArray, 0) == REG_NOMATCH)
	{
		LOG(LOG_DEBUG, "Success to match url. match pos form %d to %d " 
			, matchArray[0].rm_eo , matchArray[0].rm_so);

		int matchLen = matchArray[0].rm_eo - matchArray[0].rm_so + 2;
		char* url = new char[matchLen];
		memset(url , 0 , matchLen * sizeof(char));
		memcpy(url , pString + matchArray[0].rm_so , matchLen - 1);

		LOG(LOG_DEBUG, "url is %s " , url);

		regfree(&re);
		return 0;
	}

	regfree(&re);

	return -1;
}




