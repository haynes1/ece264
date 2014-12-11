#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include "answer12.h"

typedef struct ValueAndRange
{
	uint128 n;
	uint128 start;
	uint128 end;
	int prime;
}ValueAndRange;

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
	free(str);
	return rev;
}

ValueAndRange * ValueAndRange_construct(uint128 value, uint128 start, uint128 end){
	ValueAndRange * v = malloc(sizeof(ValueAndRange));
	v->n = value;
	v->start = start;
	v->end = end;
	v->prime = 0;
	return v;
}
//returns 0 
void * primeTest(void * t){
	int i;
	ValueAndRange * v = (ValueAndRange *)t;
	//iterate through the range testing 
	for (i = v->start; i < v->end; i++)
	{
		if ((v->n % i) == 0)//found a factor(therefore n isn't prime)
		{
			v->prime = 1;
		}
	}
	return (void*)&v->prime;
}

int primalityTestParallel(uint128 value, int n_threads){

	int i;
	int isprime;
	if (value == 0)
	{
		printf("0 is neither prime nor non-prime\n");
	}
	else if (value == 1 || value == 2 || (value % 2) == 0)
	{
		isprime = 1;
	}

	else
	{
		ValueAndRange * valueandrange_array[n_threads];
		uint128 square_root = (uint128)sqrt((double) value);
		int start;
		int end = 1;
		int range = (square_root - 2) / n_threads;
		int ret;
		pthread_t * threads = malloc(sizeof(pthread_t) * n_threads);
		//create and start all threads
		for (i = 0; i < n_threads; i++)
		{
			start = end + 1;
			end = start + range;
			valueandrange_array[i] = ValueAndRange_construct(value, start, end);
			ret = pthread_create(&threads[i], NULL, primeTest, valueandrange_array[i]);
			if(ret != 0)
			{
				printf ("Create pthread error!\n");
			}
			isprime = valueandrange_array[i]->prime;
		}
		//join all threads
		for (i = 0; i < n_threads; i++)
		{
			ret = pthread_join(threads[i], NULL);
			if (ret != 0)
			{
				printf ("ERROR ; pthread_join () returns %d\n", ret);
				return EXIT_FAILURE ;
			}
		}
		//see if the number is prime and free everything
		isprime = 0;
		for (i = 0; i < n_threads; i++)
		{
			isprime += valueandrange_array[i]->prime;
			free(valueandrange_array[i]);
		}
		free(threads);
	}

	return isprime;
}
