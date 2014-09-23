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
	char * str3 = "This is Sparta!exploded";
	int arrLen = 0;
	int i;
	const char* delims = "! ";
	printf("This is Sparta!exploded, delims = ' ', and !\n");
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
	char *  message[len];//creating an array of string literals which can't be modified
	 message[0] = "I";
	 message[1] = "Simply";
	 message[2] = "Want";
	 message[3] = "This";
	 message[4] = "Sorted";

	char ** msg = (char**)malloc(sizeof(char*)*len);//create an array of strings that aren't literals
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

	printf("Sort String Characters Testing--------------------\n");
	printf("\nBefore Sorting: %s\n", str3 );
	sortStringCharacters(str3);
	printf("After Sorting: %s\n", str3);


	printf("\n");
	return EXIT_SUCCESS;
}