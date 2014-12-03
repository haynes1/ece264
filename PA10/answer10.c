#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "answer10.h"

#define MAXLINE 500
#define MAXREVIEW 10000
#define MAXADDRESS 150
#define MAXCITY 40
#define MAXSTATE 3
#define MAXZIP 6
#define MAXID 6

typedef struct BusinessTree
{
	long offset;
	int id;
	char * name;
	char * state;
	char * zip_code; 
	struct BusinessTree * next;
	struct BusinessTree * left;
	struct BusinessTree * right;
}BusinessTree;

struct YelpDataBST
{
	BusinessTree * business_tree;
	long ** review_offsets;
	char * businesses_path;
	char * reviews_path;
};

static BusinessTree * BusinessTree_construct(int id, char * name, 
						char* state, char * zip_code, long offset)
{
	BusinessTree * tn ;
	tn = malloc(sizeof(BusinessTree));
	tn -> offset = offset;
	tn -> id = id;
	tn -> name = name;
	tn -> state = state;
	tn -> zip_code = zip_code;
	tn -> left = NULL;
	tn -> right = NULL;
	tn -> next = NULL;
	return tn;
}

BusinessTree * Tree_insert (BusinessTree * tn , int id, char * name, 
							char* state, char * zip_code, long offset)
{
	// empty , create a node
	if ( tn == NULL )
	{
		return BusinessTree_construct(id, name, state, zip_code, offset);
	}
	// not empty
	if (strcmp(name, tn -> name) == 0)//the names are equal
	{
		tn->next = Tree_insert(tn->left , id, name, state, zip_code, offset);
	}
	else if (strcmp(name, tn->name) < 0)//insert to the left
	{
		tn->left = Tree_insert(tn->left , id, name, state, zip_code, offset);
	}
	else if (strcmp(name, tn -> name) > 0)//insert to the right
	{
		tn->right = Tree_insert(tn->right , id, name, state, zip_code, offset);
	}
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
	char id[MAXID];
	char name[100];
	char state[MAXSTATE];
	char zip_code[MAXZIP];
	char * line = malloc(sizeof(char) * MAXLINE);
	int cell_index = 0;
	int i = 0;
	int iId;//the integer form of the id
	BusinessTree * root = NULL;

	while (!feof(businesses_stream)) //reads through the entire file getting lines
	{
		offset = ftell(businesses_stream);
		fgets(line, MAXLINE, businesses_stream);
		//fputs(line, stderr);
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
		//fprintf(stderr, "%d: offset = %li, Id = %d, name = %s, state = %s, zip_code = %s\n", i, offset, iId, name, state, zip_code);

		//create node of BST and populate it with the data
		iId = atoi(id);
		root = Tree_insert(root, iId, name, state, zip_code, offset);
		i++;
	}

	//reading in reviews.tsv to create an array of business IDs and offsets
	int num_reviewpositions = 2000;
	long ** review_idsandoffsets = malloc(num_reviewpositions * sizeof(long));
	char rid[6];
	int pos;
	i = 0;
	int num_rows = 0;
	int irid; //the integer form of the business id

	while (!feof(reviews_stream))
	{
		offset = ftell(reviews_stream);
		fgets(line, MAXREVIEW, reviews_stream);
		//fputs(line, stderr);
		pos = 0;
		char * previousrID = strdup(rid);
		//read in the business ID to id
		while(line[pos] != '\t')
		{
			rid[pos] = line[pos];
			pos++;
			rid[pos] = '\0';
		}
		//printf("offset = %li, previousrID = %s, rid = %s\n", offset, previousrID, rid);
		if (strcmp(previousrID, rid) != 0) //if it is the first instance of a business ID
		{
			//printf("will add a row %d with buisiness id %s\n",num_rows, rid );
			irid = atoi(rid);
			//if there is room in the array
			if (i < num_reviewpositions)
			{
				review_idsandoffsets[i] = malloc(2 * sizeof(long));
				review_idsandoffsets[i][0] = (long)irid;
				review_idsandoffsets[i][1] = offset;
			}
			//else there is no room in the array
			else{ 
				num_reviewpositions *= 2;
				review_idsandoffsets = realloc(review_idsandoffsets, num_reviewpositions);
				review_idsandoffsets[i] = malloc(2 * sizeof(long));
				review_idsandoffsets[i][0] = (long)irid;
				review_idsandoffsets[i][1] = offset;
			}
			num_rows++;
			printf("business id = %li, offset = %li\n", review_idsandoffsets[i][0], review_idsandoffsets[i][1]);
		}

		i++;
	}

	//creating the YelpDataBST
	struct YelpDataBST * thedata;
	thedata = malloc(sizeof(struct YelpDataBST));
	thedata->business_tree = root;
	thedata->review_offsets = review_idsandoffsets;
	return NULL;
}

struct BusinessTree * searchTree(BusinessTree * tn , char * name)
{
	//can't find
	if ( tn == NULL )
	{
		return NULL ;
	}
	// found
	if (strcmp(name, tn->name) == 0)
	{
		return tn ;
	}
	//search left side
	if(strcmp(name, tn->name) < 0)
	{
		return searchTree(tn -> left , name);
	}
	else if(strcmp(name, tn->name) > 0)
	{
		return searchTree(tn -> right , name);
	}

	return NULL;	
}

struct Location * locationConstruct(BusinessTree * tn, long offset, char * businesses_path)
{
	struct Location * loc = NULL;
	//open file to read in remaining strings
	FILE * businesses_stream = NULL;
	businesses_stream = fopen(businesses_path, "r");
	char * line = malloc(sizeof(char) * MAXLINE);
	char address[MAXADDRESS];
	char city[MAXCITY];
	while (!feof(businesses_stream)) //reads through the entire file getting lines
	{
		fgets(line, MAXLINE, businesses_stream);
		int b = 0;
		int a = 0;
		int cell_index = 0;
		
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
				case 2:
					address[b] = line[a];
					b++;
					address[b] = '\0';
					break;
				case 3:
					city[b] = line[a];
					b++;
					city[b] = '\0';
					break;
				default:
					break;
			}

			a++;
		}
		loc->address = address;
		loc->city = city;
	}	

	loc->state = tn->state;
	loc->zip_code = tn->zip_code;
	loc->reviews = NULL;

	return loc;
}

int compar(const void * a, const void * b)
{
	const struct Location *l1 = (struct Location *)a;
	const struct Location *l2 = (struct Location *)b;
	char * l1str = strdup(l1->state); 
	strcat(l1str, l1->city);
	strcat(l1str, l1->address);
	char * l2str = strdup(l2->state); 
	strcat(l2str, l2->city);
	strcat(l2str, l2->address);

	return(strcmp(l1str, l2str));
}

struct Review * reviewConstruct(int stars, char* text)
{
	struct Review * review = NULL;
	review->stars = (uint8_t)stars;
	review->text = text;
	return review;
}

struct Business* get_business_reviews(struct YelpDataBST* bst,
                                      char* name, char* state, char* zip_code)
{
	//create the structs
	BusinessTree * bnode = NULL;
	struct Business * business = NULL;
	struct Location * locations = NULL;
	struct Review * reviews = NULL;
	BusinessTree * bnodetraverse;
	char * reviews_path = bst->reviews_path;

	//search the tree for the business name and populate business
	bnode = searchTree(bst->business_tree, name);
	bnodetraverse = bnode;
	business->name = bnode->name;

	//traverse through the bnode linked list of locations and populate the locations array
	int locationbuffersize = 10;
	locations = malloc(locationbuffersize * sizeof(struct Location));
	int num_locations = 0;
	while(bnode != NULL)
	{
		//ensure that the array is large enough
		if (num_locations >= (locationbuffersize - 1))//suspicious stuff here
		{
			locationbuffersize *= 2;
			locations = realloc(locations, locationbuffersize * sizeof(struct Location));
		}
		locations[num_locations] = *locationConstruct(bnode, bnode->offset, bst->businesses_path);
		bnode = bnode->next;
		num_locations++;
	}
	business->num_locations = num_locations;

	//sort the array of locations by state >> city >> address
	qsort(locations, (size_t)num_locations, sizeof(struct Location), compar);

	//initializing the review array
	int i;
	int reviewbuffsize = 10;
	reviews = malloc(reviewbuffsize * sizeof(struct Review));
	long busidoffset;

	//create and populate the review array for each location
	char * line = malloc(sizeof(char) * MAXREVIEW);
	for (i = 0; i < num_locations; i++)
	{
		//get the business id, and offset for a location
		int busid = bnodetraverse->id;
		busidoffset = bnodetraverse->offset;
		//open reviews.tsv for reading, and seek the first instance of the buisinessID
		FILE * reviews_stream = NULL;
		reviews_stream = fopen(reviews_path, "r");
		fseek(reviews_stream, busidoffset, SEEK_SET);
		//parse the stream for all of the reviews and populate the array
		int num_reviews = 0;
		while(!feof(reviews_stream))
		{
			//get a line, and initialize all buffers that will become attributes of a Review struct
			fgets(line, MAXREVIEW, reviews_stream);
			char rid[MAXID]; //the read in business ID
			int irid; //the integer version of rid
			char stars[2]; //the number of stars
			int istars; //the integer version of stars
			char review_text[MAXREVIEW];
			int a = 0;
			int b = 0;
			int cell_index = 0;

			//read through the line populating ID, stars, and review_text
			while(line[a] != '\0')
			{
				switch(cell_index)
				{
					case 0:
						rid[b] = line[a];
						b++;
						rid[b] = '\0';
						break;
					case 1:
						stars[b] = line[a];
						b++;
						stars[b] = '\0';
						break;
					case 5:
						review_text[b] = line[a];
						b++;
						review_text[b] = '\0';
						break;
					default:
						break;
				}
				a++;	
			}
			irid = atoi(rid);
			istars = atoi(stars);
			if (irid == busid)
			{
				//ensure that the array is large enough
				if (num_reviews >= (reviewbuffsize - 1))
				{
					reviewbuffsize *= 2;
					reviews = realloc(reviews, reviewbuffsize * sizeof(struct Review));
				}
				reviews[num_reviews] = *reviewConstruct(istars, review_text);
			}
			else{
				break;
			}
		}
		//add the array of reviews to the Location struct
		locations[i].reviews = reviews;
	}

	//add the location array to the Business struct
	business->locations = locations;

	return business;
}
