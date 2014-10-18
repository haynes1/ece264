#include "answer06.h"
#include <stdio.h>

#define NORTH 1
#define EAST 2
#define SOUTH 3
#define WEST 4
#define HALLWAY 5
#define DEAD 6
#define INTERSECTION 7

//changes x and y based on moving direction d
void appendLocation(int * r, int * c, int d){

	if (d == NORTH)
	{
		*r -= 1;
	}
	else if (d == EAST)
	{
		*c += 1;
	}
	else if (d == SOUTH)
	{
		*r += 1;
	}
	else if (d == WEST)
	{
		*c -= 1;
	}
}

int findOpposite(int d){
	if (d == NORTH || d == EAST)
	{
		return (d+2);
	}
	else
	{
		return(d-2);
	}
}

int printDir(int d, int i){
	int opposite; 
	if (d == NORTH)
	{
		printf("N %d\n", i);
		opposite = SOUTH;
	}
	if (d == EAST)
	{
		printf("E %d\n", i);
		opposite = WEST;
	}
	if (d == SOUTH)
	{
		printf("S %d\n", i);
		opposite = NORTH;
	}
	if (d == WEST)
	{
		printf("W %d\n", i);
		opposite = EAST;
	}
	return opposite;
}


//returns whether or not the current space is a dead end, intersection, or hallway
int spaceType(int x, int y, int d, char ** maze, int w, int h){
	//test all adjacent spaces
	int i;
	int a;
	int b;
	int open = 0;//will be 1 if this is an intersection or a hallway
	for (i = 1; i <= WEST; i++)
	{
		a = x;
		b = y;
		appendLocation(&a, &b, i);
		//if we did not move out of the maze
		if (a < h && b < w )
		{
			if (maze[a][b] != 'X')
			{
				open++;//there is an open direction
			}
		}
	}
	a = x;
	b = y;
	appendLocation(&a, &b, d); //a and b are now in the next space given by direction d
	if (a >= h || b >= w || (open == 1 && maze[a][b] == 'X' ))//the space is a dead end, or the next space in d is an edge
	{
		return DEAD;
	}
	else if (open == 2 && maze[a][b] == ' ')//the space is part of a hallway
	{
		return HALLWAY;
	}
	else //we are at an intersection
	{
		return INTERSECTION;
	}
}

//moves in direction d
void moveDirection(int x, int y, int d, char ** maze, int w, int h){
	int type = HALLWAY;//holds the type of the current point (HALLWAY to start the loop)
	int i = 0; 
	int a = x;
	int b = y;
	while(type == HALLWAY){
		appendLocation(&a, &b, d);
		type = spaceType(a, b, d, maze, w, h);
		i++;
	}
	//print direction
	int opposite;
	opposite = printDir(d, i);

	//if the type is an intersection

	//print opposite direction to return to previous intersection
	d = printDir(opposite, i);
}

void atIntersection(int x, int y, int d, char ** maze, int w, int h){
	int i;
	for(i = NORTH; i <= WEST; i++)//iterate through all directions
	{
		if (findOpposite(i) != d)//ensure that I don't go back in the direction I came from
		{
			moveDirection(x, y, i, maze, w, h);//move in all directions except the the direction I came from
		}
	}
}



void print_directions(char** maze, int w, int h);