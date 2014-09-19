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
	int i;
	const char* delims = "! ";
	printf("This is Sparta mother!fuckers, delims = ' ', and !\n");
	char * * exploded = explode(str3, delims, &arrLen);
	//printing
	for (i = 0; i < arrLen; i++)
	{
		printf("%s\n", exploded[i]);
	}
	printf("\n");

	printf("implode testing --------------------------------\n");
	const char * glue = " GLUE ";
	char * imploded = implode(exploded, arrLen, glue);
	printf("%s\n\n", imploded );

	printf("sortStringArray testing --------------------------------\n");
	int len = 5;
	int j;
	char *  message[len];
	 message[0] = "Fuck";
	 message[1] = "This";
	 message[2] = "Shit";
	 message[3] = "I am";
	 message[4] = "Done";

	char ** msg = (char**)malloc(sizeof(char*)*len);
	for(j = 0; j < len; j++){
		msg[j] = (char*)malloc((strlen(message[j]) + 1) * sizeof(char));
		msg[j] = strcpy(msg[j], message[j]);
	}

	for (j = 0; j < len; j++)
	{
		printf("%s\n", msg[j]);
	}

	printf("AFTER SORting\n");

	sortStringArray(msg, len);
	//printing
	for (j = 0; j < len; j++)
	{
		printf("%s\n", msg[j]);
	}

	printf("\n");
	return EXIT_SUCCESS;
}