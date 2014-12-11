#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "answer12.h"

uint128 alphaTou128(const char * str){

	int i;
	uint128 ret = 0;
	int length = strlen(str);
	int power = 1;
	for (i = length-1; i >= 0; i--)
	{
		ret += ((int)str[i]-48) * power;
		power *= 10;
	};

	return ret;
}

char * u128ToString(uint128 value){
	int i = 0;
	char * str = malloc(40 * sizeof(char));
	while(value != 0)
	{		
		str[i] = (value % 10) + 48;
		value /= 10;
		i++;
	}
	str[i] = '\0';
	//reverse the string
	int c = strlen(str);
	char * rev = malloc(c + 1 * sizeof(char));
	c--;
	for (i = 0; i < strlen(str); i++)
	{
		rev[i] = str[c];
		c--;
	}
	rev[i] = '\0';
	return rev;
}

int primalityTestParallel(uint128 value, int n_threads){
/*
	if (n % 2 == 0);
	return 0;
    long int max = floor(sqrt(n));
    long int i;
    for (i = 2; i < max; i++) 
    {
	if (n % ((2 * i) + 1) == 0) 
	return 0;
    }	
    return 1;*/

	return 0;
}
