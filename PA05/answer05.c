#include "answer05.h"
#include <stdlib.h>
#include <string.h>

//returns the number of partitions for a given n
int nParts(int n){
	int i;
	int sum = 0;
	if (n == 1)
	{
		return 1; //only one way to partition 1
	}
	for (i = 1; i < n; i++)
	{
		sum += nParts(n - i);
	}
	sum++;
	return sum;
}

void recurTime (int value, int * * buffer, int column){

	int i;
	int j;
	int c = 0;
	int k = 0;

	if (value == 0) //base case
	{
		return;
	}

	for (i = 1; i <= value; i++) //doing it! iterating 1 - (value-1)
	{
		for (j = c; j < (nParts(value - i) + c); j++) //puts the numbers in front of the next call of recurTime
		{
			buffer[j][column] = i;
			k++;//keeping track of which row we are on
		}
		c = k;//the next time the above loop iterates it will start with our current row
		if (i == value)
		{
			buffer[c - 1][column] = value;
		}
	}

}

void partitionAll(int value){

	int * * buffer; //make and dynamically allocate array
    buffer = (int * *) malloc(nParts(value) * sizeof(int *));
    int i;
    for (i = 0; i < nParts(value); i++)
    {
        buffer[i] = (int *) malloc(value * sizeof(int));
    }

	int a;
	int b;
	//int x, j; 
	//int k = 0; 
	//int c = 0;
	//int recur = 0;
	
	for (a = 0; a < nParts(value); a++)//setting array to 0
	{
		for (b = 0; b < value; b++)
		{
			buffer[a][b] = 0;
		}
	}

	recurTime(value, buffer, 0);

	/*for (x = 1; x <= value; x++) //doing it! iterating 1 - (value-1)
	{
		for (j = c; j < (nParts(value - x) + c); j++) //puts the numbers in front of the next call
		{
			buffer[j][recur] = x;
			k++;//keeping track of which row we are on
		}
		c = k;//the next time the above loop iterates it will start with our current row
		if (x == value)
		{
			buffer[c][recur] = value;
		}

	} */

	for (a = 0; a < nParts(value); a++)//printing the  array
	{
		for (b = 0; b < value; b++)
		{
			printf("%d, ", buffer[a][b]);
		}
		printf("\n");
	}

	return;
}

/**
 * Prints all partitions that have strictly increasing values.
 *
 * Example:
 * partitionIncreasing(5); // prints the following: (line order not important)
 * = 1 + 4
 * = 2 + 3
 * 
 * These partitions are excluded because they are not _strictly_ increasing
 * 1 + 1 + 3
 * 2 + 1 + 2
 * 3 + 2
 * 
 * Note:
 * The program should generate only valid partitions.  Do not
 * generates all partitions and then filter for validity. If you 
 * do this, you will almost certainly have trouble understanding the exam.
 *
 * Hint: look at file: expected/partitionIncreasing.output
 */
void partitionIncreasing(int value){

}

/**
 * Prints all partitions that have strictly decreasing values.
 *
 * Example:
 * partitionDecreasing(5); // prints the following: (line order not important)
 * = 3 + 2
 * = 4 + 1
 * = 5
 * 
 * These partitions are excluded because they are not _strictly_ decreasing
 * 1 + 1 + 3
 * 2 + 1 + 2
 * 
 * See: note on partitionIncreasing(...)
 * Hint: look at file: expected/partitionDecreasing.output
 */
void partitionDecreasing(int value){

}

/**
 * Prints all partitions comprised solely of odd numbers.
 *
 * Example:
 * partitionOdd(5); // prints the following (line order not important)
 * = 1 + 1 + 1 + 1 + 1
 * = 1 + 1 + 3
 * = 1 + 3 + 1
 * = 3 + 1 + 1
 * = 5
 * 
 * See: note on partitionIncreasing(...)
 * Hint: for odd numbers, (value % 2 != 0)
 * Hint: look at file: expected/partitionOdd.output
 */
void partitionOdd(int value){

}

/**
 * Prints all partitions comprised solely of even numbers.
 *
 * Example:
 * partitionEven(6); // prints the following (line order not important)
 * = 2 + 2 + 2
 * = 2 + 4
 * = 4 + 2
 * = 6
 * 
 * See: note on partitionIncreasing(...)
 * Hint: for even numbers, (value % 2 == 0)
 * Hint: you can never partition an odd number with even numbers alone.
 * Hint: look at file: expected/partitionEven.output
 */
void partitionEven(int value){

}

/**
 * Prints all partitions that do not have consecutive odd or even numbers.
 * In other words, it only prints partitions that have alternating odd and 
 * even numbers.
 *
 * Example:
 * partitionOddAndEven(5); // prints the following (line order not important)
 * = 1 + 4
 * = 2 + 1 + 2
 * = 2 + 3
 * = 3 + 2
 * = 4 + 1
 * = 5
 *
 * See: note on partitionIncreasing(...)
 * Hint: look at file: expected/partitionOddAndEven.output
 */
void partitionOddAndEven(int value){

}

/**
 * Prints all partitions that comprise solely of prime numbers.
 *
 * Example:
 * partitionPrime(5); // prints the following (line order not important)
 * = 2 + 3
 * = 3 + 2
 * = 5
 * 
 * See: note on partitionIncreasing(...)
 * Hint: you will need to write a function that checks if a number is prime.
 * Hint: 1 is not a prime number.
 * Hint: look at file: expected/partitionPrime.output
 */
void partitionPrime(int value){

}