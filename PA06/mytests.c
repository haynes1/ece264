#include <stdio.h>
#include <stdlib.h>
#include "answer06.c"

#define NORTH 1
#define EAST 2
#define SOUTH 3
#define WEST 4
#define HALLWAY 5
#define DEAD 6
#define INTERSECTION 7

int main(int argc, char * * argv){
	printf("\n-------------------------Testing Time--------------------------\n");
	int x = 2;
	int y = 2;
	int d = SOUTH;

	//testing appendLocation
	printf("current location: (%d, %d)\n", x, y);
	appendLocation(&x, &y, d);
	printf("new location: (%d, %d)\n", x, y);

	//building a testing maze for spaceType
	char ** maze = malloc(5 * sizeof(char *));
	int i;
	for (i = 0; i < 5; i++)
	{
		maze[i] = malloc(5 * sizeof(char));
	}
	maze[0] = "X X X";
	maze[1] = "X X X";
	maze[2] = "X   X";
	maze[3] = "X X X";
	maze[4] = "X XXX";


	printf("--------------------Testing spaceType--------------------\n");
	int type;
	//printing the maze
	for (i = 0; i < 5; i++)
	{
		printf("%s\n", maze[i]);
	}
	type = spaceType(1, 3, d, maze, 5, 5);
	printf("the type is: %d\n", type);
	type = spaceType(2, 3, d, maze, 5, 5);
	printf("the type is: %d\n", type);
	type = spaceType(3, 3, d, maze, 5, 5);
	printf("the type is: %d\n", type);
	type = spaceType(4, 1, d, maze, 5, 5);
	printf("the type is: %d\n", type);

	printf("---------------------Testing moveDirection----------------------\n");
	printf("starting point: (0, 1)\n");
	moveDirection(0, 1, d, maze, 5, 5);

	printf("---------------------Testing atIntersection-----------------------\n");
	maze[0] = "XX XX";
	maze[1] = "XX XX";
	maze[2] = "     ";
	maze[3] = "XX XX";
	maze[4] = "XX XX";
	//printing the maze
	for (i = 0; i < 5; i++)
	{
		printf("%s\n", maze[i]);
	}
	atIntersection(2,2,d,maze,5,5);

	return EXIT_SUCCESS;
}