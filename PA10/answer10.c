#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "answer10.h"

#define MAXLINE 500

typedef struct BusinessNode
{
	long offset;
	char * name;
	char * state;
	char * zip_code; 
	struct BusinessNode * next;
	struct BusinessNode * left;
	struct BusinessNode * right;
}BusinessTree;

struct YelpDataBST
{
	struct BusinessBST * BusinessBST;
	long ** review_offsets; //need to learn the types of this 2D array
};

static BusinessTree * BusinessTree_construct(char * id, char * name, char* state, char * zip_code, long offset)
{
	BusinessTree * tn ;
	tn = malloc(sizeof(BusinessTree));
	tn -> offset = offset;
	tn -> name = name;
	tn -> state = state;
	tn -> zip_code = zip_code;
	tn -> left = NULL;
	tn -> right = NULL;
	tn -> next = NULL;
	return tn;
}

struct YelpDataBST* create_business_bst(const char* businesses_path,
                                        const char* reviews_path)
{
	// open both files for reading and ensure that they both worked
	FILE * businesses_stream = NULL;
	FILE * reviews_stream = NULL;
	businesses_stream = fopen(businesses_path, "r");
	reviews_stream = fopen(reviews_path, "r");
	if (businesses_stream == NULL || reviews_stream == NULL)
	{
		printf("one or both of the files failed to open. Cannot create BST\n");
		return NULL;
	}

	//reading in businesses.tsv and creating the BST
	long offset;
	char id[6];
	char name[100];
	char state[3];
	char zip_code[6];
	char * line = malloc(sizeof(char) * MAXLINE);
	int cell_index = 0;
	int i = 0;

	while (!feof(businesses_stream)) //reads through the entire file getting lines
	{
		offset = ftell(businesses_stream);
		fgets(line, MAXLINE, businesses_stream);
		fputs(line, stderr);
		int b = 0;
		int a = 0;
		cell_index = 0;
		
		//parse each line to get needed data
		while(line[a] != '\0')
		{
			if (line[a] == '\t') //test to see if we've moved to a new cell
			{
				cell_index++;
				b = 0;
				a++; //to move past the '\t'
			}
			switch(cell_index)
			{
				case 0:
					id[b] = line[a];
					b++;
					id[b] = '\0';
					break;
				case 1:
					name[b] = line[a];
					b++;
					name[b] = '\0';
					break;
				case 4:
					state[b] = line[a];
					b++;
					state[b] = '\0';
					break;
				case 5:
					zip_code[b] = line[a];
					b++;
					zip_code[b] = '\0';
					break;
				default:
					break;

			}
			a++;
		}
		fprintf(stderr, "%d: Id = %s, name = %s, state = %s, zip_code = %s\n", i, id, name, state, zip_code);

		//create node of BST and populate it with the data


		i++;
	}






	//reading in reviews.tsv to create a 2D array of businesses ID's and their corresponding file offsets

	//creating the YelpDataBST
	return NULL;
}
