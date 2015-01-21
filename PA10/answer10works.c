#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "answer10.h"

#define MAXLINE 500
#define MAXREVIEW 100000
#define MAXADDRESS 150
#define MAXCITY 40
#define MAXSTATE 3
#define MAXZIP 6
#define MAXID 6

typedef struct BusinessTree
{
	long offset;
	long first_reviewoffset;
	int id;
	char * name;
	struct BusinessTree * next;
	struct BusinessTree * left;
	struct BusinessTree * right;
}BusinessTree;

struct YelpDataBST
{
	BusinessTree * business_tree;
	char * businesses_path;
	char * reviews_path;
	int total_businesses;
};

static BusinessTree  BusinessTree_construct(int id, char * name, long offset, long first_reviewoffset)
{
	BusinessTree tn;
	tn.offset = offset;
	tn.first_reviewoffset = first_reviewoffset;
	tn.id = id;
	tn.name = strdup(name);
	tn.left = NULL;
	tn.right = NULL;
	tn.next = NULL;
	return tn;
}

BusinessTree * buildTree(int * array, BusinessTree * barray, int beginning, int end)
{
	if (barray == NULL || array == NULL || beginning >= end)
	{
		return NULL;
	}
	int mid = (end + beginning) / 2;
	int ind = array[mid];
	//printf("id: %d, name: %s, state: %s, zip_code: %s, offset: %li\n", barray[ind].id, barray[ind].name, barray[ind].state, 
		//barray[ind].zip_code, barray[ind].offset);
	//make the left
	barray[ind].left = buildTree(array, barray, beginning, mid);
	//make the right
	barray[ind].right = buildTree(array, barray, mid+1, end);

	return (&barray[ind]);
}

/*static void printTree(BusinessTree * thetree)
{
	if ( thetree == NULL )
	{
		return;
	}
	printTree(thetree->left);
	printf("id: %d, name: %s offset: %li, firstreview: %li next: %p, left: %p right: %p\n",
		thetree->id, thetree->name, thetree->offset, thetree->first_reviewoffset, 
		thetree->next, thetree->left, thetree->right);
	printTree(thetree->next);
	printTree(thetree->right);
}*/

int comparb(const void * a, const void * b)
{
	const struct BusinessTree *l1 = (BusinessTree *)a;
	const struct BusinessTree *l2 = (BusinessTree *)b;

	return(strcasecmp(l1->name, l2->name));
}

/* Returns the offset of the first review for the given location-id, or -1 if there are no reviews */

long get_offset_of_first_review(FILE * reviews_stream, int id)
{
	long offset;
	int line_position;
	char * line = malloc(sizeof(char) * MAXREVIEW);
	char rid[MAXID];
	int read_id;
	int i = 0;
	if (reviews_stream != NULL)
	{
		while (!feof(reviews_stream))
		{
			offset = ftell(reviews_stream);
			fgets(line, MAXREVIEW, reviews_stream);
			//fputs(line, stderr);
			line_position = 0;
			//read in the business ID to id
			while(line[line_position] != '\t')
			{
				rid[line_position] = line[line_position];
				line_position++;
				rid[line_position] = '\0';
			}
			read_id = atoi(rid);
			//printf("read id = %d, looking for id %d\n", read_id, id);

			 //if it is the first instance of a business ID
			if (read_id == id)
			{
				//printf("FOUND id %d, looking for id %d\n", read_id, id );
				free(line);
				return offset;
				break;
			}
			else if (read_id > id)
			{
				free(line);
				fseek(reviews_stream, offset, SEEK_SET);
				return -1;
			}
			i++;
		}
	}
	else
	{
		printf("reviews.tsv failed to open\n");
	}
	free(line);
	return -1;
}

//DESTROY HELPER FUNCTIONS
BusinessTree * getLefty(BusinessTree * b)
{
	BusinessTree * lefty;
	//base case
	if (b->left == NULL)
	{
		return b;
	}
	lefty = getLefty(b->left);
	return lefty;
}

int numNodes(BusinessTree *n)
{
    int c = 1;
 
    if (n == NULL)
        return 0;
    else
    {
        c += numNodes(n->left);
        c += numNodes(n->right);
        return c;
    }
}

void destroyBusinessTree(BusinessTree * t, int num_nodes)
{	
	//frees this as though it were an array starting from lefty
	BusinessTree * lefty = getLefty(t);
	//printf("lefty's name is: %s\n", lefty->name);
	int i;
	for (i = 0; i < num_nodes; ++i)
	{
		//test to see if the node goes deep, free its stuff
		if (lefty[i].next != NULL)
		{
			BusinessTree * a = lefty->next;
			while(a != NULL)
			{
				BusinessTree * n = a->next;
				free(a->name);
				free(a);
				a = n;
			}
		}
		free(lefty[i].name);
		//printf("freeing lefty's %dth node\n", i );
	}
	free(lefty);
	return;
}

void destroyReviews(struct Review * r, int num_reviews)
{
	int i;
	for (i = 0; i < num_reviews; i++)
	{
		free(r[i].text);
	}
	return;
}

void destroyLocations(struct Location * l, int num_locations)
{
	int i;
	for (i = 0; i < num_locations; i++)
	{
		free(l[i].address);
		free(l[i].city);
		free(l[i].state);
		free(l[i].zip_code);
		destroyReviews(l[i].reviews, l[i].num_reviews);
		free(l[i].reviews);
	}
	return;
}


struct YelpDataBST* create_business_bst(const char* businesses_path,
                                        const char* reviews_path)
{
	// open both files for reading and ensure that they both worked
	FILE * businesses_stream = NULL;
	FILE * reviews_stream = NULL;
	businesses_stream = fopen(businesses_path, "r");
	reviews_stream = fopen(reviews_path, "r");
	if (businesses_stream == NULL)
	{
		printf("business.tsv failed to open\n");
		return NULL;
	}
	//reading in businesses.tsv and creating the BST
	long offset;
	char id[MAXID];
	char name[100];
	char * line = malloc(sizeof(char) * MAXLINE);
	int cell_index = 0;
	int num_businesses = 0;
	int iId;//the integer form of the id
	//BusinessTree * root = NULL;
	int treearraylength = 1000;
	BusinessTree * treearray = malloc(treearraylength * sizeof(BusinessTree));
	while (!feof(businesses_stream))
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
				default:
					break;
			}
			a++;
		}
		//fprintf(stderr, "%d: offset = %li, Id = %d, name = %s, state = %s, zip_code = %s\n", i, offset, iId, name, state, zip_code);

		//create node of BST and populate it with the data
		iId = atoi(id);
		//ensure there in enough room in the array
		if (num_businesses > (treearraylength -1))
		{
			treearraylength *= 2;
			treearray = realloc(treearray, treearraylength * sizeof(BusinessTree));
		}
		//add data to array
		treearray[num_businesses] = BusinessTree_construct(iId, name, offset, get_offset_of_first_review(reviews_stream, iId)); 
		num_businesses++;
	}
	free(line);
	fclose(businesses_stream);
	fclose(reviews_stream);
	//sorting the array by name
	qsort(treearray, (size_t)num_businesses, sizeof(BusinessTree), comparb);
	//free(treearray[num_businesses-1].name);
	int i;
	/*for (i = 0; i < num_businesses-1; i++)
	{
		printf("%d) id: %d, name: %s, offset: %li, first review: %li\n", i,
			treearray[i].id, treearray[i].name, treearray[i].offset, treearray[i].first_reviewoffset);
	}*/
	//free that annoying last component of the array

	//creating new array as well as handle duplicates of the same name
	int uniquenames[num_businesses-1];
	int uniquenamespos = 0;
	//BusinessTree * print = malloc(sizeof(BusinessTree));
	//BusinessTree * tmp;
	for (i = 0; i < num_businesses-2; i++)
	{
		//read current and next business names
		char * nameA = treearray[i].name;
		char * nameB = treearray[i+1].name;
		//print = &treearray[i];
		//the names are the same, add the next treenode to the list
		if (strcasecmp(nameA, nameB) == 0)
		{
			//add to unique names list, and increment number of unique names
			uniquenames[uniquenamespos] = i;
			uniquenamespos++;
			//increment i, and add nodes to list untill nameA != name B
			int listlength = 0;
			while(strcasecmp(nameA, nameB) == 0)
			{
				treearray[i].next = &treearray[i+1];
 				i++;
 				nameB = treearray[i+1].name;
 				listlength++;
			}
		}
		else
		{
			uniquenames[uniquenamespos] = i;
			uniquenamespos++;
		}
	}

	//print the int array uniquenames
	/*for (i = 0; i < uniquenamespos; i++)
	{
		printf("%d\n", uniquenames[i]);
	}*/
	//building the BST
	buildTree(uniquenames, treearray, 0, uniquenamespos);
	int mid = (uniquenamespos) / 2;
	int ind = uniquenames[mid];
	//printTree(&treearray[ind]);

	//creating the YelpDataBST
	struct YelpDataBST * bst;
	bst = malloc(sizeof(struct YelpDataBST));
	bst->total_businesses = num_businesses;
	bst->business_tree = &treearray[ind];
	bst->businesses_path = (char*)businesses_path;
	bst->reviews_path = (char*)reviews_path;
	//printf("================================Tree!!!!!!!!!!!========================\n");
	//printTree(bst->business_tree);
	return bst;
}

struct BusinessTree * searchTree(BusinessTree * tn , char * name)
{
	//can't find
	if ( tn == NULL )
	{
		return NULL ;
	}
	// found
	if (strcasecmp(name, tn->name) == 0)
	{
		//printf("found node: name = %s, id = %d, offset = %li, next = %p\n", tn->name, tn->id, tn->offset, tn->next);
		return tn ;
	}
	//search left side
	if(strcasecmp(name, tn->name) < 0)
	{
		return searchTree(tn -> left , name);
	}
	else if(strcasecmp(name, tn->name) > 0)
	{
		return searchTree(tn -> right , name);
	}

	return NULL;	
}

struct Review  createReview(int stars, char * text)
{
	//printf("stars: %s, text: %s\n", stars, text);
	struct Review  r;
	r.text = strdup(text);
	r.stars = (uint8_t)stars;

	return r;
}

int compar(const void * a, const void * b)
{
	const struct Location *l1 = (struct Location *)a;
	const struct Location *l2 = (struct Location *)b;
	int comparison;
	comparison = strcmp(l1->state, l2->state);
	if (comparison == 0)
	{
		comparison = strcmp(l1->city, l2->city);
		if (comparison == 0)
		{
			comparison = strcasecmp(l1->address, l2->address);
			if (comparison == 0)
			{
				return 1;
			}
		}
	}

	return(comparison);
}

int comparReviews(const void * a, const void * b)
{
	const struct Review *l1 = (struct Review *)a;
	const struct Review *l2 = (struct Review *)b;
	int comparison;
	if (l1->stars < l2->stars)
	{
		comparison = 1;
	}
	else if (l1->stars == l2->stars)
	{
		comparison = strcasecmp(l1->text, l2->text);
	}
	else if (l1->stars > l2->stars)
	{
		comparison = -11;
	}

	return comparison;
}

struct Location  locationConstruct(BusinessTree * tn, char * businesses_path)
{
	struct Location loc;
	//open file to read in remaining strings
	FILE * bstream;
	bstream = fopen(businesses_path, "r");
	char * line = malloc(sizeof(char) * MAXLINE);
	char address[MAXADDRESS];
	char city[MAXCITY];
	char state[MAXSTATE];
	char zip_code[MAXZIP];
	if (bstream != NULL)
	{
		fseek(bstream, tn->offset, SEEK_SET);
		fgets(line, MAXLINE, bstream);
		//fputs(line, stderr);
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
		loc.address = strdup(address);
		loc.city = strdup(city);
		loc.state = strdup(state);
		loc.zip_code = strdup(zip_code);	
	}
	else
	{
		char *error_str = strerror(errno);
		printf("%s\n", error_str);
	}
	//printf("address: %s, city: %s, state: %s, zip: %s\n", loc->address, loc->city, loc->state, loc->zip_code);
	loc.reviews = NULL;
	fclose(bstream);
	free(line);
	return loc;
}

struct Business* get_business_reviews(struct YelpDataBST* bst,
                                      char* name, char* state, char* zip_code)
{
	//int i;
	//create the structs and open the streams
	BusinessTree * found_node;
	struct Business * b = malloc(sizeof(struct Business));
	struct Location * locations = NULL; //array of locations for the Business struct
	struct Review ** reviews = NULL;
	FILE * reviews_stream = fopen(bst->reviews_path, "r");
	//printTree(bst->business_tree);

	//search the tree for the business name and populate business
	found_node = searchTree(bst->business_tree, name);
	//could not find business
	if (found_node == NULL)
	{
		return NULL;
	}
	//print to ensure that the linked list worked

	b->name = found_node->name;

	//populate Business business
	b->locations = NULL;
	int locationbuffersize = 10;
	locations = malloc(locationbuffersize * sizeof(struct Location));
	int num_locations = 0;
	int num_reviews = 0;
	reviews = malloc(locationbuffersize * sizeof(struct Review *));
	//BusinessTree * tmp;
	//create array of locations
	while(found_node != NULL)
	{
		//ensure that the array is large enough
		if (num_locations >= (locationbuffersize - 1))//suspicious stuff here
		{
			locationbuffersize *= 2;
			locations = realloc(locations, locationbuffersize * sizeof(struct Location));
			reviews = realloc(reviews, locationbuffersize * sizeof(struct Review *));
		}
		struct Location l = locationConstruct(found_node, bst->businesses_path);
		//test to see if the location fits the bill
		
		if (state != NULL && strcmp(l.state, state) != 0)
		{
			BusinessTree * tmp = found_node->next;
			free(l.state);
			free(l.city);
			free(l.zip_code);
			free(l.address);
			/*free(found_node->name);
			free(found_node);*/
			found_node = tmp;
			continue;
		}
		if (zip_code != NULL && strcmp(l.zip_code, zip_code) != 0)
		{
			BusinessTree * tmp = found_node->next;
			free(l.state);
			free(l.city);
			free(l.zip_code);
			free(l.address);
			/*free(found_node->name);
			free(found_node);*/
			found_node = tmp;
			continue;
		}
		locations[num_locations] = l;
		//add the array of reviews to each location
		char id[MAXID];
		char stars[2];
		char text[MAXREVIEW];
		char * line = malloc(sizeof(char) * MAXREVIEW);
		int cell_index;
		int maxreviews = 10;
		reviews[num_locations] = malloc(maxreviews * sizeof(struct Review));
		fseek(reviews_stream, found_node->first_reviewoffset, SEEK_SET);
		num_reviews = 0;
		//get lines, create review structs, add to array
		do
		{
			fgets(line, MAXREVIEW, reviews_stream);
			if(feof(reviews_stream))
			{
				break;
			}
			//fputs(line, stderr);
			int c = 0;
			int a = 0;
			cell_index = 0;
			
			//parse each line to get needed data
			while(line[a] != '\0')
			{
				if (line[a] == '\t') //test to see if we've moved to a new cell
				{
					cell_index++;
					c = 0;
					a++; //to move past the '\t'
				}
				switch(cell_index)
				{
					case 0:
						id[c] = line[a];
						c++;
						id[c] = '\0';
						break;
					case 1:
						stars[c] = line[a];
						c++;
						stars[c] = '\0';
						break;
					case 5:
						text[c] = line[a];
						c++;
						text[c] = '\0';
						break;
					default:
						break;
				}
				a++;
			}
			text[c-1] = '\0';

			//ensure there in enough room in the array
			if (atoi(id) == found_node->id)
			{
				if (num_reviews > (maxreviews -1))
				{
					maxreviews *= 2;
					reviews[num_locations] = realloc(reviews[num_locations], maxreviews * sizeof(struct Review));
				}
				//add data to array
				reviews[num_locations][num_reviews] = createReview(atoi(stars), text);
				num_reviews++;
				//printf("id: %d, stars: %d, text: %s\n", atoi(id), atoi(stars) , text);
			}

		}while(atoi(id) == found_node->id && found_node->id != bst->total_businesses);
		free(line);

		//sorting the array of reviews
		qsort(reviews[num_locations], (size_t)num_reviews, sizeof(struct Review), comparReviews);

		//populating locations
		locations[num_locations].reviews = reviews[num_locations];
		locations[num_locations].num_reviews = num_reviews;
		//tmp = found_node;
		found_node = found_node->next;
		//free(tmp);
		num_locations++;
	}
	fclose(reviews_stream);
	free(reviews);
	b->num_locations = num_locations;

	//sort the array of locations by state >> city >> address and print
	qsort(locations, (size_t)num_locations, sizeof(struct Location), compar);

	//add the location array to the Business struct
	b->locations = locations;
	b->num_locations = num_locations;
	if (num_locations == 0)
	{
		return NULL;
	}

	return b;
}

void destroyTheNames(BusinessTree * n)
{
	if (n == NULL)
	{
		return;
	}
	destroyTheNames(n->left);
	destroyTheNames(n->next);
	destroyTheNames(n->right);
	free(n->name);
	return;
}

void destroy_business_bst(struct YelpDataBST* bst)
{
	//destroyTheNames(bst->business_tree);
	destroyBusinessTree(bst->business_tree, bst->total_businesses);
	//free(bst->businesses_path);
	//free(bst->reviews_path);
	free(bst);
	return;
}
/* Deallocate all memory allocated for the object returned by
 * create_business_bst(..) and close the files. */

void destroy_business_result(struct Business* b){
	//int i;
	//free(b->name);
	/*printf("locations: %p, num_locations: %d\n",b->locations, b->num_locations);
	for (i = 0; i < b->num_locations; i++)
	{
		printf("address: %s, city: %s, state: %s, zip: %s, reviews: %p, numrevs: %d\n",
			b->locations[i].address,  b->locations[i].city,  b->locations[i].state,  b->locations[i].zip_code,  
			b->locations[i].reviews, b->locations[i].num_reviews);
	}*/
	destroyLocations(b->locations, b->num_locations);
	free(b->locations);
	free(b);
	return;
}