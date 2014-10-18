#include "answer06.h"
#include <stdio.h>

#define NORTH 1
#define EAST 2
#define SOUTH 3
#define WEST 4
#define HALLWAY 5
#define DEAD 6
#define INTERSECTION 7

//appends x and y based on moving direction d
void appendLocation(int * x, int * y, int d){
	if (d == NORTH)
	{
		*y -= 1;
	}
	else if (d == EAST)
	{
		*x += 1;
	}
	else if (d == SOUTH)
	{
		*y += 1;
	}
	else if (d == WEST)
	{
		*x -= 1;
	}
}

//returns whether or not the current space is a dead end, intersection, or hallway
//int spaceType(int x, int y, int d){


//}