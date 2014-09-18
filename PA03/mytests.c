#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "answer03.h"

int main (int argc, char * * argv)
{
	printf("\nThis is my testing program for PA03\n");

	char * str1 = "Something";
	const char * str2 = "Anything";
	int len1 = strlen(str1);
	//int len2 = strlen(str2);

	//strcat_ex testing
	char * catenated = strcat_ex(&str1, &len1, str2);
	printf("strcat_ex testing-----------------------------\n");
	printf("concatenation: %s\n", catenated);
	printf("\n");

	printf("explode testing --------------------------------\n");
	char * str3 = "This is Sparta mother!fuckers";
	int arrLen = 0;
	const char* delims = "! ";
	printf("This is Sparta mother!fuckers, delims = ' ', and !\n");
	char * * exploded = explode(str3, delims, &arrLen);
	printf("%p\n", exploded); 


	printf("\n");
	return EXIT_SUCCESS;
}