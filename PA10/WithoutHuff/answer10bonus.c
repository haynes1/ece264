#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "answer10bonus.h"

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

typedef struct ReviewOffset
{
	int id;
	long offset;
}ReviewOffset;

struct YelpDataBST
{
	BusinessTree * business_tree;
	int mid_index;
	char * businesses_path;
	char * reviews_path;
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

	//make the left
	barray[ind].left = buildTree(array, barray, beginning, mid);
	//make the right
	barray[ind].right = buildTree(array, barray, mid+1, end);

	return (&barray[ind]);
}

int comparb(const void * a, const void * b)
{
	const struct BusinessTree *l1 = (BusinessTree *)a;
	const struct BusinessTree *l2 = (BusinessTree *)b;

	return(strcmp(l1->name, l2->name));
}

int comparrevoff(const void * a, const void * b)
{
	const struct ReviewOffset *l1 = *(ReviewOffset **)a;
	const struct ReviewOffset *l2 = *(ReviewOffset **)b;
	int comparison;
	if (l1->id < l2->id)
	{
		comparison = -1;
	}
	else if (l1->id == l2->id)
	{
		comparison = 0;
	}
	else if (l1->id > l2->id)
	{
		comparison = 1;
	}

	return comparison;
}

ReviewOffset ** reviewIdsandOffsets(FILE * reviews_stream, int * num){
	//make all needed variables
	int num_unique_ids = 0;
	int max_unique_ids = 500;
	ReviewOffset ** ret_array = malloc(max_unique_ids * sizeof(ReviewOffset *));
	//make variables for line reading and read in first id to current and add to array
	ret_array[num_unique_ids] = malloc(sizeof(ReviewOffset));
	char * line = malloc(MAXREVIEW * sizeof(char));
	int previous_read_id;
	char * current_read_id = malloc(MAXID * sizeof(char));
	long current_offset;
	current_offset = ftell(reviews_stream);
	fgets(line, MAXREVIEW, reviews_stream);
	//fputs(line, stderr);
	int a = 0;
	while(line[a] != '\t')
	{
		current_read_id[a] = line[a];
		current_read_id[a+1] = '\0';
		a++;
	}
	ret_array[num_unique_ids]->id = atoi(current_read_id);
	ret_array[num_unique_ids]->offset = current_offset;
	//printf("%d) id: %li, offset: %li\n", num_unique_ids, ret_array[num_unique_ids][0], ret_array[num_unique_ids][1]);
	num_unique_ids++;
	//reading in the rest of the ids
	while(!feof(reviews_stream))
	{
		previous_read_id = atoi(current_read_id);
		current_offset = ftell(reviews_stream);
		fgets(line, MAXREVIEW, reviews_stream);
		if (line == NULL)
		{
			free(line);
			break;
		}
		//fputs(line, stderr);
		//read in current id
		a = 0;
		while(line[a] != '\t')
		{
			current_read_id[a] = line[a];
			current_read_id[a+1] = '\0';
			a++;
		}
		//test to see if the currint business id is new/unique
		if (previous_read_id != atoi(current_read_id))
		{
			//ensure there is enough room in the array
			if (num_unique_ids >= max_unique_ids-1)
			{
				max_unique_ids *= 2;
				ret_array = realloc(ret_array, max_unique_ids * sizeof(long *));
			}
			ret_array[num_unique_ids] = malloc(sizeof(ReviewOffset));
			ret_array[num_unique_ids]->id = atoi(current_read_id);
			ret_array[num_unique_ids]->offset = current_offset;
	
			num_unique_ids++;
		}

	}
	free(line);
	free(current_read_id);

	*num = num_unique_ids;

	return ret_array;
}

/* Returns the offset of the first review for the given location-id, or -1 if there are no reviews */

long get_offset_of_first_review(int id, ReviewOffset ** array, int start, int end)
{
	int mid_index = (end + start) / 2;
	int mid_id = array[mid_index]->id;
	long offset;
	if (start > end)
	{
		return -1;
	}
	if (mid_id == id)
	{
		offset = array[mid_index]->offset;
		return offset;
	}
	//id is in the lower half of the array
	else if (id < mid_id)
	{
		offset = get_offset_of_first_review(id, array, start, mid_index - 1);
	}
	//id is in the upper half of the array
	else if (id > mid_id)
	{
		offset = get_offset_of_first_review(id, array, mid_index + 1, end);
	}
	return offset;
}


struct YelpDataBST* create_business_bst(const char* businesses_path,
                                        const char* reviews_path, int * total_businesses)
{
	// open both files for reading and ensure that they both worked
	FILE * businesses_stream = NULL;
	FILE * reviews_stream = NULL;
	businesses_stream = fopen(businesses_path, "r");
	reviews_stream = fopen(reviews_path, "r");
	if (businesses_stream == NULL ||reviews_stream == NULL)
	{
		printf("one or both of the files failed to open\n");
		return NULL;
	}
	//reading getting the array of review offsets
	ReviewOffset ** review_idandoffsets;
	int num_reviewoffsets;
	review_idandoffsets = reviewIdsandOffsets(reviews_stream, &num_reviewoffsets);
	//sorting the array of review offsets, and sending the no longer needed data to the end of the array
	qsort(review_idandoffsets, (size_t)num_reviewoffsets, sizeof(ReviewOffset *), comparrevoff);
	//printing the array to ensure it worked

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
		

		//create node of BST and populate it with the data
		iId = atoi(id);
		//ensure there in enough room in the array
		if (num_businesses > (treearraylength -1))
		{
			treearraylength *= 2;
			treearray = realloc(treearray, treearraylength * sizeof(BusinessTree));
		}
		//add data to array
		long first_reviewoffset = get_offset_of_first_review(iId, review_idandoffsets, 0, num_reviewoffsets);
		treearray[num_businesses] = BusinessTree_construct(iId, name, offset, first_reviewoffset); 
		num_businesses++;
	}
	free(line);
	fclose(businesses_stream);
	fclose(reviews_stream);
	//freeing the review_idandoffset array
	int i;
	for (i = 0; i < num_reviewoffsets; i++)
	{
		free(review_idandoffsets[i]);
	}
	free(review_idandoffsets);

	//sorting the array by name
	qsort(treearray, (size_t)num_businesses-1, sizeof(BusinessTree), comparb);
	free(treearray[num_businesses-1].name);
	//free that annoying last component of the array

	*total_businesses = num_businesses-1;

	//creating new array as well as handle duplicates of the same name
	int uniquenames[num_businesses-1];
	int uniquenamespos = 0;
	BusinessTree * current;
	for (i = 0; i < num_businesses-2; i++)
	{
		//read current and next business names
		char * nameA = treearray[i].name;
		char * nameB = treearray[i+1].name;
		current = &treearray[i];
		//the names are the same, add the next treenode to the list
		if (strcmp(nameA, nameB) == 0)
		{
			//add to unique names list, and increment number of unique names
			uniquenames[uniquenamespos] = i;
			uniquenamespos++;
			//increment i, and add nodes to list untill nameA != name B
			int listlength = 0;
			while(strcmp(nameA, nameB) == 0)
			{
 				i++;
 				current->next = &treearray[i];
 				nameB = treearray[i+1].name;
 				listlength++;
			}
			//print out the linked list

		}
		else
		{
			uniquenames[uniquenamespos] = i;
			uniquenamespos++;
		}
	}

	//print the int array uniquenames

	//building the BST
	buildTree(uniquenames, treearray, 0, uniquenamespos);
	int mid = (uniquenamespos) / 2;
	int ind = uniquenames[mid];

	//creating the YelpDataBST
	struct YelpDataBST * bst;
	bst = malloc(sizeof(struct YelpDataBST));
	bst->business_tree = treearray;
	bst->mid_index = ind;
	bst->businesses_path = (char*)businesses_path;
	bst->reviews_path = (char*)reviews_path;
	//printf("================================Tree!!!!!!!!!!!========================\n");

	return bst;
}

//===================================Search Functions=================================
//helper function 0 strstr: case insensitive check to see if the string starts with the prefix
int foundPrefix(char * haystack, char * needle)
{
	int i;
	for (i = 0; i < strlen(needle); i++)
	{
		if (tolower(haystack[i]) != tolower(needle[i]))
		{
			return 0;
		}
	}

	return 1;
}

//helper function 1
BusinessTree * findPrefixTree(BusinessTree * n, char * prefix)
{
	if (strstr(n->name, prefix) == 0 || strcmp(prefix, n->name) == 0)
	{
		return n;
	}
	//go left
	else if (strcmp(prefix, n->name) < 0)
	{
		findPrefixTree(n->left, prefix);
	}
	//go right
	else if (strcmp(prefix, n->name) > 0)
	{
		findPrefixTree(n->right, prefix);
	}
	//not found
	return NULL;
}

//helper function 2
void populateBusinessArray(BusinessTree ** array, BusinessTree * t, char * prefix, int * num_businesses, int max_businesses)
{
	//base case
	if (t == NULL)
	{
		return;
	}
	//found the prefix
	if(foundPrefix(t->name, prefix) == 1)
	{
		//ensure there is enough space in the array of businesses
		if (*num_businesses >= max_businesses -1)
		{
			max_businesses *= 2;
			array = realloc(array, max_businesses * sizeof(BusinessTree *));
		}
		array[*num_businesses] = t;
		int numb = *num_businesses;
		numb++;
		* num_businesses = numb; //increment num_businesses
	}
	//go left, then right
	populateBusinessArray(array, t->left, prefix, num_businesses, max_businesses);
	populateBusinessArray(array, t->right, prefix, num_businesses, max_businesses);
	return;
}

struct BusinessTree ** searchTree(BusinessTree * tn , char * prefix, int * num_businesses)
{
	//printf("num busin: %d\n", *num_businesses);
	int max_businesses = 50;
	BusinessTree ** array = malloc(max_businesses * sizeof(BusinessTree *));
	//find the first node containing the prefix
	BusinessTree * first_node = findPrefixTree(tn, prefix);
	if (first_node == NULL)
	{
		return NULL;
	}
	//populate the populateBusinessArray
	populateBusinessArray(array, first_node, prefix, num_businesses, max_businesses);

	return array;	
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
			comparison = strcmp(l1->address, l2->address);
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
		comparison = strcmp(l1->text, l2->text);
	}
	else if (l1->stars > l2->stars)
	{
		comparison = -1;
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


struct Review * getReviews(FILE * reviews_stream, int id, long first_reviewoffset, int * num_rev, char ** words, int num_words)
{
	int maxreviews = 10;
	struct Review * reviews = malloc(maxreviews * sizeof(struct Review));
	char read_id[MAXID];
	char stars[2];
	char text[MAXREVIEW];
	char * line = malloc(sizeof(char) * MAXREVIEW);
	if (line == NULL)
	{
		printf("failed to malloc a line in function: getReviews\n");
		return NULL;
	}
	int cell_index;
	fseek(reviews_stream, first_reviewoffset, SEEK_SET);
	int num_reviews = 0;
	int contains_words; //1 if the string contains the words
	int word_iterator;
	//get lines and do the thing to them
	do
	{
		contains_words = 0;
		fgets(line, MAXREVIEW, reviews_stream);
		int c = 0;
		int a = 0;
		cell_index = 0;
		//check to see if the line contains the words (if yes, contains_words = num_words)
		if (words != NULL && num_words != 0)
		{
			for (word_iterator = 0; word_iterator < num_words; word_iterator++)
			{
				if (strstr(line, words[word_iterator]) != NULL)
				{
					contains_words++;
				}
			}
		}
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
					read_id[c] = line[a];
					c++;
					read_id[c] = '\0';
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
		//ensure the line contains the proper business id
		if (atoi(read_id) == id && contains_words == num_words)
		{
			//ensure there in enough room in the array of reviews
			if (num_reviews > (maxreviews -1))
			{
				maxreviews *= 2;
				reviews = realloc(reviews, maxreviews * sizeof(struct Review));
			}
			//add data to array
			reviews[num_reviews] = createReview(atoi(stars), text);
			num_reviews++;
			//printf("id: %d, stars: %d, text: %s\n", atoi(read_id), atoi(stars) , text);
		}
	}while(atoi(read_id) == id);
	free(line);
	*num_rev = num_reviews;
	return reviews;
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


struct Business* populateBusiness(BusinessTree * node, char* name, char * businesses_path,
		 char* reviews_path, char * state, char * zip_code, char ** words, int num_words, int * num_businesses)
{
	int i;
	//create the structs and open the streams
	struct Business * b = malloc(sizeof(struct Business));
	struct Location * locations = NULL; //array of locations for the Business struct
	struct Review ** reviews = NULL;
	FILE * reviews_stream = fopen(reviews_path, "r");
	b->name = strdup(node->name);

	b->locations = NULL;
	int locationbuffersize = 10;
	locations = malloc(locationbuffersize * sizeof(struct Location));
	int num_locations = 0;
	int num_reviews = 0;
	reviews = malloc(locationbuffersize * sizeof(struct Review *));
	//BusinessTree * tmp;
	//create array of locations
	while(node != NULL)
	{
		//ensure that the array is large enough
		if (num_locations >= (locationbuffersize - 1))//suspicious stuff here
		{
			locationbuffersize *= 2;
			locations = realloc(locations, locationbuffersize * sizeof(struct Location));
			reviews = realloc(reviews, locationbuffersize * sizeof(struct Review *));
		}
		locations[num_locations] = locationConstruct(node, businesses_path);
		//add the array of reviews to each location
		reviews[num_locations] = getReviews(reviews_stream, node->id, 
		node->first_reviewoffset, &num_reviews, words, num_words);

		//sorting the array of reviews
		qsort(reviews[num_locations], (size_t)num_reviews, sizeof(struct Review), comparReviews);
		/*for (i = 0; i < num_reviews; i++) //testing review array
		{
			printf("id: %d, stars: %d, text: %s\n", found_node->id, reviews[num_locations][i].stars, reviews[num_locations][i].text);
		}*/
		//populating locations
		locations[num_locations].reviews = reviews[num_locations];
		locations[num_locations].num_reviews = num_reviews;
		//tmp = found_node;
		node = node->next;
		//free(tmp);
		num_locations++;
	}
	fclose(reviews_stream);
	free(reviews);
	b->num_locations = num_locations;

	//filter by state and zip code
	int j;
	for (i = 0; i < num_locations; ++i)
	{
		if (state != NULL)
		{
			if (strcmp(state, locations[i].state) != 0)
			{
				for (j = 0; j < num_locations-1; j++)
				{
					locations[i] = locations[i+1];
				}
				free(locations[i].address);
				free(locations[i].city);
				free(locations[i].state);
				free(locations[i].zip_code);
				free(locations[i].reviews);
				destroyReviews(locations[i].reviews, num_reviews);
				free(&locations[i]);
				num_locations--;
			}
		}
		if (zip_code != NULL)
		{
			if (strcmp(state, locations[i].state) != 0)
			{
				for (j = 0; j < num_locations-1; j++)
				{
					locations[i] = locations[i+1];
				}
				free(locations[i].address);
				free(locations[i].city);
				free(locations[i].state);
				free(locations[i].zip_code);
				free(locations[i].reviews);
				destroyReviews(locations[i].reviews, num_reviews);
				free(&locations[i]);
				num_locations--;
			}	
		}
	}

	//sort the array of locations by state >> city >> address and print
	qsort(locations, (size_t)num_locations, sizeof(struct Location), compar);


	//add the location array to the Business struct
	b->locations = locations;
	printf("name: %s, locations: %p, num_locations: %d\n", b->name, b->locations, b->num_locations);
	for (i = 0; i < b->num_locations; i++)
	{
		printf("address: %s, city: %s, state: %s, zip: %s, reviews: %p, numrevs: %d\n",
			b->locations[i].address,  b->locations[i].city,  b->locations[i].state,  b->locations[i].zip_code,  
			b->locations[i].reviews, b->locations[i].num_reviews);
	}
	return b;
}

struct Business ** get_business_reviews(struct YelpDataBST* bst,
                                      char* name, char* state, char* zip_code, char ** words, int num_words, int *num_businesses)
{
	
	int i;
	//create the structs and open the streams
	BusinessTree ** found_array;
	struct BusinessTree * top_node = &(bst->business_tree[bst->mid_index]);
	//printf("the top node is) id: %d, name: %s, left: %p, right: %p\n", top_node->id,top_node->name,top_node->left,top_node->right);
	//create array of BusinessTree * and find the number of businesses, and malloc the Business array
	found_array = searchTree(top_node, name, num_businesses);
	if (found_array == NULL)
	{
		return NULL;
	}
	struct Business ** b_array = malloc(*num_businesses * sizeof(struct Business *));

	//populate the businesses
	for (i = 0; i < *num_businesses; i++)
	{
		b_array[i] = populateBusiness(found_array[i], found_array[i]->name, bst->businesses_path, bst->reviews_path, state, zip_code, 
			words, num_words, num_businesses);
	}
	free(found_array);

	return b_array;
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

void destroy_business_bst(struct YelpDataBST* bst, int num_businesses)
{
	//destroyTheNames(bst->business_tree);
	destroyBusinessTree(bst->business_tree, num_businesses);
	free(bst);
	return;
}
/* Deallocate all memory allocated for the object returned by
 * create_business_bst(..) and close the files. */

void destroy_business_result(struct Business** b, int num_businesses){
	int i;
	for (i = 0; i < num_businesses; ++i)
	{
		free(b[i]->name);
		destroyLocations(b[i]->locations, b[i]->num_locations);
		free(b[i]->locations);
		free(b[i]);
	}
	free(b);
	return;
}


void noNameYesWords(char ** words, int num_words, const char * reviews_path)
{
 	printf("Searched for words without a business name. This will take some time\n");
 	FILE * reviews_stream = fopen(reviews_path, "r");
 	if (reviews_stream == NULL)
 	{
 		printf("failed to open reviews.tsv\n");
 	}
 	//parse each line of reviews.tsv for the stuff	char read_id[MAXID];
	char stars[2];
	char text[100000];
	char * line = malloc(sizeof(char) * 100000);
	if (line == NULL)
	{
		printf("failed to malloc a line in function: getReviews\n");
		return;
	}
	int cell_index;
	int num_reviews = 0;
	int contains_words; //1 if the string contains the words
	int word_iterator;
	//get lines and do the thing to them
	while(!feof(reviews_stream))
	{
		contains_words = 0;
		fgets(line, 100000, reviews_stream);
		int c = 0;
		int a = 0;
		cell_index = 0;
		//check to see if the line contains the words (if yes, contains_words = num_words)
		if (words != NULL && num_words != 0)
		{
			for (word_iterator = 0; word_iterator < num_words; word_iterator++)
			{
				if (strstr(line, words[word_iterator]) != NULL)
				{
					contains_words++;
				}
			}
		}
		if(contains_words == num_words)
		{
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
			printf("Review %d) Stars = %d\n", num_reviews, atoi(stars));
			printf("%s\n", text);
			num_reviews++;
		}
	}
	free(line);

 	return;
}

