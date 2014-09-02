#include <stdlib.h>
#include "answer01.h"
#include <stdio.h>

int arraySum(int * array, int len)
{
	int sum = 0;
	int i;

    for(i = 0; i < len; ++i) {

    	sum += array[i];
}
    return sum;
}


int arrayCountNegative(int * array, int len)
{
	int negs = 0;
	int i;

    for(i = 0; i < len; ++i) {

    	if (array[i] < 0){

    		negs++;
    	}
}
    return negs;
}

int arrayIsIncreasing(int * array, int len)
{   
	int up = 1;
	int i;
	int x;

    for(i = 1; i < len; ++i) {

    	if (array[i] >= array[i-1]){

    		up++;
    	}

}

    if (up == len){
    	x = 1;
    }
    else{
    	x = 0;
    }
    return x;
}

int arrayIndexRFind(int needle, const int * haystack, int len)
{
	int ind;
	int i;

	ind = -1;

	for(i = 0; i < len; ++i){

		if(haystack[i] == needle){

			ind = i;
		}
	}
    return ind;
}

int arrayFindSmallest(int * array, int len)
{
	int mindex;
	int i;

	mindex = 1;

	for(i = 0; i < len-1; ++i){

		if (array[i] < array[mindex])
		{
			mindex = i;
		}

		
	}

	if (array[0] <= array[1])
	{
		mindex = 0;
	}

    return mindex;
}
