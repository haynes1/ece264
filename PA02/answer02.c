#include "answer02.h"

size_t my_strlen(const char * str)
{
	int i;

	i = 0;

	while (str[i] != '\0'){

		i++;
	}

	
   return i;
}

int my_countchar(const char * str, char ch)
{
	int i;
	int c;

	i = 0;
	c = 0;

	while (str[i] != '\0'){

		if (str[i] == ch)
		{
			c++;
		}

		i++;
	}

	return c;
}

char * my_strchr(const char * str, int ch)
{
	int i;

	i = 0;

	while (str[i] != '\0'){

		if (str[i] == ch)
		{
			return (char*)(str+i);
		}

		i++;
	}

	return NULL; //ch was not found in string

	
}

char * my_strrchr(const char * str, int ch)
{

	return 0;
}

char * my_strstr(const char * haystack, const char * needle)
{

	return 0;
}

char * my_strcpy(char * dest, const char * src)
{

	return 0;
}

char * my_strcat(char * dest, const char * src)
{

	return 0;
}

int my_isspace(int ch)
{

	return 0;
}

int my_atoi(const char * str)
{

	return 0;
}
