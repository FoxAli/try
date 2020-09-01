#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc,char **argv)
{
	const char* seed = "https://www.163.com/";

	char path[256] = {0};
	getcwd(path , 256);
	strcat(path , "/download");
	printf("filePath: %s\n", path);
	

	
	
	return 0;
}

