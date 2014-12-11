#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <string.h>
#include "answer10bonus.h"

#define MAXREVIEW 100000
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

/*typedef struct ReviewOffset
{
	int id;
	long offset;
}ReviewOffset;


//==================================FUNCTION NEEDED TO ALLOW reviews.tsv TO BE RANDOM
//the first line of the returned array will contain the number of reviews, followed by a 0
//the first part of the array is in heap memory, but each line is not malloced
ReviewOffset ** reviewIdsandOffsets(FILE * reviews_stream){
	//make all needed variables
	int num_unique_ids = 1;
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
			//printf("%d) id: %d, offset: %li\n", num_unique_ids, ret_array[num_unique_ids].id, ret_array[num_unique_ids].offset);
			num_unique_ids++;
		}

	}
	free(line);
	free(current_read_id);
	ret_array[0] = malloc(sizeof(ReviewOffset));
	ret_array[0]->id = num_unique_ids;
	ret_array[0]->offset = 5;

	return ret_array;
}

long bbsearch(int id, ReviewOffset ** array, int start, int end)
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
		offset = bbsearch(id, array, start, mid_index - 1);
	}
	//id is in the upper half of the array
	else if (id > mid_id)
	{
		offset = bbsearch(id, array, mid_index + 1, end);
	}
	return offset;
}
struct Review  createReview(int stars, char * text)
{
	//printf("stars: %s, text: %s\n", stars, text);
	struct Review  r;
	r.text = strdup(text);
	r.stars = (uint8_t)stars;

	return r;
}

struct Review * getReviews(FILE * reviews_stream, int id, long first_reviewoffset, int * num_rev, char ** words, int num_words)
{
	printf("made it to getReviews\n");
	int maxreviews = 10;
	struct Review * reviews = malloc(maxreviews * sizeof(struct Review));
	char read_id[MAXID];
	char stars[2];
	char text[MAXREVIEW];
	char * line = malloc(sizeof(char) * MAXREVIEW);
	int cell_index;
	fseek(reviews_stream, first_reviewoffset, SEEK_SET);
	printf("made it past fseek\n");
	int num_reviews = 0;
	int contains_words; //1 if the string contains the words
	int word_iterator;
	//get lines and do the thing to them
	do
	{
		contains_words = 0;
		fgets(line, MAXREVIEW, reviews_stream);
		fputs(line, stderr);
		int c = 0;
		int a = 0;
		cell_index = 0;
		//check to see if the line contains the words (if yes, contains_words = num_word-1)
		if (words != NULL && num_words != 0)
		{
			printf("doing the word thing\n");
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
		contains_words = num_words -1;
		if (atoi(read_id) == id && contains_words == (num_words - 1))
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
			printf("id: %d, stars: %d, text: %s\n", atoi(read_id), atoi(stars) , text);
		}
	}while(atoi(read_id) == id);
	free(line);
	*num_rev = num_reviews;
	return reviews;
}*/

struct BusinessTree * searchTreeFirst(BusinessTree * tn, char * prefix)
{
	//can't find
	if ( tn == NULL )
	{
		return NULL ;
	}
	// found
	if (strstr(tn->name, prefix) == 0)
	{
		return tn ;
	}
	//search left side
	if(strcasecmp(prefix, tn->name) < 0)
	{
		return searchTreeFirst(tn -> left , prefix);
	}
	else if(strcasecmp(prefix, tn->name) > 0)
	{
		return searchTreeFirst(tn -> right , prefix);
	}

	return NULL;	
}

void findOtherBusinesses(BusinessTree ** array, BusinessTree * first, char * prefix, int * num_businesses)
{
	//if it should be in the array, add it to the array
	if (strstr(first->name, prefix) == 0)
	{
		array[*num_businesses] = first;
		*num_businesses++;
	}
	//go left and populate
	findOtherBusinesses(array, first->left, prefix, num_businesses);
	//go right and populate
	findOtherBusinesses(array, first->right, prefix, num_businesses);
	return;
}

struct BusinessTree ** searchTreee(BusinessTree * tn, char * prefix, int * num_businesses)
{
	int max_businesses = 50;
	BusinessTree ** array = malloc(max_businesses * sizeof(BusinessTree *));
	//get the first node with prefix. All other nodes must be children of this node
	BusinessTree * first = searchTreeFirst(tn, prefix);
	findOtherBusinesses(array, first, prefix, num_businesses);

	return NULL;	
}

int main(int argc, char *argv[]){
	int i;
	//printf("====================== Testing Create BST ====================\n");
	char * businesses_path = "/home/cheif/ece264/solutions/ece264/PA10/businesses.short.tsv";
	char * reviews_path = "/home/cheif/ece264/solutions/ece264/PA10/reviews.short.tsv";
	struct YelpDataBST * bst = NULL;
	bst = create_business_bst(businesses_path, reviews_path);
	printf("testing search by prefix\n");
	int num_businesses = 0;
	/*BusinessTree * tree = bst->business_tree;
	BusinessTree ** prefix_search_results = searchTreee(tree, "Star", &num_businesses);
	for (i = 0; i < num_businesses; i++)
	{
		printf("name is: %s\n", prefix_search_results[i]->name);
	}*/



	printf("====================== Testing The Search ====================\n");
	struct Business * b;
	char ** words = malloc(4 * sizeof(char *));
	words[0] = "vanilla";
	words[1] = "frap";
	words[2] = "with";
	words[3] = "banana";
	b = get_business_reviews(bst, "Starbucks", NULL, NULL, words, 4);
	destroy_business_bst(bst);
	destroy_business_result(b);
	

	//testing the array of ids and ints
	/*ReviewOffset ** ridoff;
	FILE * reviews_stream = fopen(reviews_path, "r");
	ridoff = reviewIdsandOffsets(reviews_stream);
	fclose(reviews_stream);
	long rows = ridoff[0]->id;
	int i;
	//printing the array
	for (i = 0; i < rows; i++)
	{
		printf("id: %d, offset: %li\n", ridoff[i]->id, ridoff[i]->offset);
	}
	long offset;
	int id = 12;
	offset = bbsearch(id, ridoff, 0, rows);
	printf("id is: %d, offset: %li\n", id, offset);
	//testing the searching of words
	char ** words = malloc(4 * sizeof(char *));
	int num_words = 4;
	words[0] = "vanilla";
	words[1] = "frap";
	words[2] = "with";
	words[3] = "banana";

	struct Review * reviews;
	int num_reviews;
	reviews = getReviews(reviews_stream, (int)18, (long)349241, &num_reviews, words, num_words);

	for (i = 0; i < num_reviews; i++)
	{
		printf("stars: %d, text: %s\n", reviews[i].stars, reviews[i].text);
	}

	//freeing the array
	for (i = 0; i < rows; i++)
	{
		free(ridoff[i]);
	}
	free(ridoff);*/

	return EXIT_SUCCESS;
}

//WHERE I AM
/*I think I need to make getReviews return an array of Review * instead of an array of Reviews so that it works...
make the code work if there is no business, but it does search for words in reviews*/