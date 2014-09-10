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
	
	//iterates through str
	while (str[i] != '\0'){

		if (str[i] == ch)
		{
			return (char*)(&str[i]);
		}

		i++;
	}

	return NULL;
}

char * my_strrchr(const char * str, int ch){

	int i;

	i = my_strlen(str);

	while (i >= 0){

		if (str[i] == ch)
		{
			return (char*)(&str[i]);
		}

		i--;
	}

	return NULL; //ch was not found in string


}


char * my_strstr(const char * haystack, const char * needle)
{

	int i; 


	int j = 0;

	while(haystack[i] != '\0'){ //iterates through haystack

		j = 0;
		while(haystack[i+j] == needle[j] && needle[j] != '\0'){ //compares wherever we are in haystack to the beginning of needle
			j++;
		}

		if(needle[j] == '\0'){ //made it to the end of needle, therefore the needle is in the haystack
		
			return (char*)(haystack + i);
		}else{
			i++;
		}

	}

	
	return NULL; //no needle found

}

char * my_strcpy(char * dest, const char * src)
{
	int i; //iterator

	i = 0;

	while (src[i] != '\0'){
		//copying
		dest[i] = src[i];
		i++;
	}

	i++;

	dest[i] = '\0';

	return dest;
}

char * my_strcat(char * dest, const char * src)
{
	int i;
	int j;
	int len;

	i = 0;
	j = 0;
	len = my_strlen(dest) + my_strlen(src);

	while (i <= len){

		if (dest[i] == '\0') //if the end of dest is reached
		{
			dest[i] = src[j]; //set the end of dest to part of src
			dest[i+1] = '\0'; //move the end of dest over one to accomodate part of src
			j++; //move the iterator for src forward one
		}

		i++;

	}



	return dest;
}

int my_isspace(int ch)
{

	if (ch == ' ' || ch == '\f' || ch == '\n' || ch == '\r' || ch == '\t' || ch == '\v')
	{
		return 1;
	}
	else{

		return 0;
	}
}

int my_atoi(const char * str)
{
	int i;
	int ret;
	int minus;

	i = 0;
	ret = 0;
	minus = 0;

	//will take to the start of the numbers if they exist
	while(my_isspace(str[i]) == 1){

		i++;
	}

	if (str[i] == '-') //if there is a minus sign, iterate past it
	{
		i++;
		minus = 1;

	}

	while ('0' <= str[i] && str[i] <= '9'){

		ret = ret * 10;
		ret = ret + (str[i] - '0');
		i++;
	}

	if (minus == 1)
	{
		return -ret;
	}
	else
	{
		return ret;
	}

}