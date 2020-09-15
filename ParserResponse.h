#ifndef PARSER_RESPONSE_H
#define PARSER_RESPONSE_H

enum EHyperlinkType
{
	Absolutely,
	Relative
};

int ParserString(char* pString, int* findPos, EHyperlinkType* hyperlinkType);


#endif


