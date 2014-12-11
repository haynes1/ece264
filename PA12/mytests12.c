#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "answer12.h"

int main (int argc, char * * argv)
{
	printf("============================Testing alphaTou128=======================\n");
	uint128 number;
	const char * string = "24354657";
	number = alphaTou128(string);
	printf("string is: %s, number is: %li\n", string, (long)number);
	printf("============================Testing u128ToString=======================\n");
	uint128 n = 456789;
	char * s = u128ToString(n);
	printf("number %li converts to: %s\n", (long)n, s);


	return EXIT_SUCCESS;
}