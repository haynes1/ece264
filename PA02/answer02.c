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


char * my_strstr(const char * haystack, const char * needle)
{
	int nlen = my_strlen(needle);

	int i; //iterator for haystack count
	int c; //iterator for needle count
	int test; //will be 0 if the needle is found in the haystack

	i = 0;
	c = 0;
	test = 0;

	while (haystack[i] != 0){  //iterates through haystack

		if (haystack[i] == needle[i]){ //if we find the first letter of needle

			c = i;

			while (c < nlen){ //will iterate through needle

				if (haystack[c] != needle[c]){ //test to see if each of the letters in needle match haystack

					test++; //will occur if there is not a match
				}

				c++;
			}
		}

		i++;
	}

	if (test == 0) //needle was found 
	{
		return (char *)needle;
	}

	else {

		return NULL;
	}


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
