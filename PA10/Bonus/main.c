#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffer.h"
#include "answer10bonus.h"

#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2
#define MAXNAME 500
#define MAXSTATE 3
#define MAXZIP 12
#define MAXWORD 50

void printBusiness(Business * b, int num);

void printLocation(Location l, int num);

void printReview(Review r, int num);

static int getLine (char *prmpt, char *buff, size_t sz) {
    int ch, extra;

    // Get line with buffer overrun protection.
    if (prmpt != NULL) {
        printf ("%s", prmpt);
        fflush (stdout);
    }
    if (fgets (buff, sz, stdin) == NULL)
        return NO_INPUT;

    // If it was too long, there'll be no newline. In that case, we flush
    // to end of line so that excess doesn't affect the next call.
    if (buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    // Otherwise remove newline and give string back to caller.
    buff[strlen(buff)-1] = '\0';
    return OK;
}

int main(int argc, char *argv[]){
	int i;

	const char businesses_path[] = "/home/cheif/ece264/solutions/ece264/PA10/businesses.short.tsv";
	const char reviews_path[] = "/home/cheif/ece264/solutions/ece264/PA10/reviews.short.tsv";

	if(argc == 1) {
		fprintf(stderr, "=================================================Starting Program=============================================\n");
		fprintf(stderr, "Run this program with a 1 as the argument to procede.\nThis will start the huffman compression.\n");
		fprintf(stderr, "It will take approximately XXX to complete, and will only happen once before multiple searches can be made.\n\n");
		return EXIT_FAILURE;
	}
	if (argc == 2 && strcmp(argv[1], "1") == 0)
	{
		fprintf(stderr, "=================================================Starting Program=============================================\n");
		fprintf(stderr, "Starting huffman compression.\n");
		fprintf(stderr, "This will take approximately XXX minutes to complete before you will be prompted for multiple searches\n");
	}


	//create the BST
	struct YelpDataBST * bst = NULL;
	int total_businesses = 0;
	bst = create_business_bst(businesses_path, reviews_path, &total_businesses);

	//prompt user, and do the searches
	int continue_searching = 1;
	char * prompt = "Search Syntax: Name/Prefix State Zip Word0 Word1 Word2....Wordn\n";
	int max_line_length = 2000;
	char * line = malloc(max_line_length * sizeof(char));
	getLine(prompt, line, 2000);
	printf("search for: %s\n", line);
	//explode the search string
	char * name = malloc(MAXNAME * sizeof(char));
	char * state = malloc(MAXSTATE * sizeof(char));
	char * zip = malloc(MAXZIP * sizeof(char));
	char * word = malloc(MAXWORD * sizeof(char));
	int max_words_in_array = 20;
	char ** word_array = malloc(max_words_in_array * sizeof(char *));
	int num_words = 0;
	int b = 0;
	int a = 0;
	int cell_index = 0;
	while(line[a] != '\0')
	{
		if (line[a] == ' ') //test to see if we've moved to a new cell
		{
			cell_index++;
			b = 0;
			a++; //to move past the ' '
		}
		switch(cell_index)
		{
			case 0:
				name[b] = line[a];
				b++;
				name[b] = '\0';
				break;
			case 1:
				state[b] = line[a];
				b++;
				state[b] = '\0';
				break;
			case 2:
				zip[b] = line[a];
				b++;
				zip[b] = '\0';
				break;
			default:
				break;
		}
		//dealing with the words
		if (cell_index > 2)
		{
			word[b] = line[a];
			b++;
			word[b] = '\0';
			//found an entire word, add it to the array
			if (line[a+1] == ' ' || line[a+1] == '\0')
			{
				//ensure the array can handle it
				if (num_words >= max_words_in_array-1)
				{
					max_words_in_array *= 2;
					word_array = realloc(word_array, max_words_in_array * sizeof(char));
				}
				word_array[num_words] = strdup(word);
				num_words++;
			}
		}
		a++;
	}
	//ensure everything that wasn't entered is null
	printf("cell_index: %d\n", cell_index );
	switch(cell_index+1)
	{
		case 0:
			name = NULL;
		case 1:
			state = NULL;
		case 2:
			zip = NULL;
		case 3:
			word_array = NULL;
		default:
			break;
	}
	/*printf("name: %s, state: %s, zip: %s, word_array: %p\n", name, state, zip, word_array);
	for (i = 0; i < num_words; ++i)
	{
		printf("%s\n", word_array[i]);
	}*/
	//using the user entered search, create the business structs
	struct Business ** businesses;
	int num_businesses = 0;
	businesses = get_business_reviews(bst, name, state, zip, word_array, num_words, &num_businesses);

	//print out the businesses
	printBusiness(businesses[0], 1);

	//free all memory
	free(word);
	free(name);
	free(state);
	free(zip);
	for (i = 0; i < num_words; ++i)
	{
		free(word_array[i]);
	}
	free(word_array);
	destroy_business_bst(bst, total_businesses);
	destroy_business_result(businesses, num_businesses);

	return EXIT_SUCCESS;
}

void printBusiness(Business * b, int num)
{
	int i;
	printf("=============================================================================================\n");
	printf("Business # %d Name: %s\n", num, b->name);
	printf("Number of Locations: %d\n", b->num_locations);
	for (i = 0; i < b->num_locations; i++)
	{
		printLocation(b->locations[i], i);
	}
	printf("---------------------------------------------------------------------------------------------\n");
	printf("=============================================================================================\n");
	return;
}

void printLocation(Location l, int num)
{
	int i;
	printf("---------------------------------------------------------------------------------------------\n");
	printf("Location %d) Address: %s\tState: %s\tCity: %s\tZip Code: %s\n",num, l.address, l.state, l.city, l.zip_code);
	for (i = 0; i < l.num_reviews; i++)
	{
		printReview(l.reviews[i], i);
	}
	return;
}

void printReview(Review r, int num)
{
	printf("Review %d)\t%d stars\n",num, r.stars);
	printf("%s\n", r.text);
	return;
}