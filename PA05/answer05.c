#include "answer05.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

void findParts(int value, int ** grid, int r, int c){

	//base cases(0)
	if (value == 0)
	{
		return;
	}
	else if (value == 1)
	{
		grid[r][c] = 1;
		return;
	}

	//non base cases
	int i;
	int j;
	int a;
	int crow = 0;
	for (i = 1; i <= value; i++)
	{
		for (j = 0; j < nParts(value - i); j++)
		{
			grid[r+crow][c] = i;
			crow ++;//this increments with the total j's moved and allows me to keep track of rows (j resets to 0 all the time)
		}
	}
	//recursion time
	int newval;
	int rcurrow = r;
	int rcurcol = c+1; //move over one column
	for (a = 0; a < value-1; a++)
	{
		newval = value-1-a;
		findParts(newval, grid, rcurrow, rcurcol);
		rcurrow += nParts(newval);
	}

	return;
}


void partitionAll(int value){

	//allocating space for array (value x value)
	int a;
	int numP;
	numP = nParts(value);
	int ** grid = malloc(numP * sizeof(int *)); //nparts(value)rows
	for (a = 0; a < numP; a++)
	{
		grid[a] = malloc(value* sizeof(int));//value colums
	}

	//calling findParts recursively
	findParts(value, grid, 0, 0);

	//printing the grid
	int b;
	int z;
	for (b = 0; b < nParts(value); b++)
	{
		for (z = 0; z < value; z++)
		{
			if (grid[b][z] <= value)//the test to see if it gets printed
			{
				printf("%d ", grid[b][z]);
			}
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