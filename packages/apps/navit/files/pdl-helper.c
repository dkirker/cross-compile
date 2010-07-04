
#include <stdio.h>
#include <PDL.h>

#define BUFSIZE 1024

int main ()
{
	char buf[BUFSIZE+1];

	PDL_Init(0);

	PDL_GetDataFilePath("", buf, BUFSIZE);
	printf("PDL_DataFilePath=\"%s\"\n", buf);
	PDL_GetLanguage(buf, BUFSIZE);
	printf("PDL_Language=\"%s\"\n", buf);

	return 1;
}
