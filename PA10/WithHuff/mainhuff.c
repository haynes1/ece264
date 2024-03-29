#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dostuffhuff.h"

#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2
#define MAXNAME 500
#define MAXSTATE 6
#define MAXZIP 12
#define MAXWORD 50

void printBusiness(Business * b, int num, char ** words, int num_words, WordData * huffman_tree);

void printLocation(Location * l, int num, char ** words, int num_words, WordData * huffman_tree);


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

	char businesses_path[] = "/home/cheif/ece264/solutions/ece264/PA10/businesses.short.tsv";
	char reviews_path[] = "/home/cheif/ece264/solutions/ece264/PA10/reviews.short.tsv";

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

	int num_huffed_words = 0;
	WordData * huffman_tree = parseAndHuff(reviews_path, &num_huffed_words);
	outputHuffmanFile(huffman_tree, reviews_path, num_huffed_words);
	deHuffer_file("huffed.txt", huffman_tree);


	//create the BST
	struct YelpDataBST * bst = NULL;
	int total_businesses = 0;
	bst = create_business_bst(businesses_path, "huffed.txt", &total_businesses, huffman_tree);

	//prompt user, and do the searches
	int continue_searching = 1;
	char * prompt = "Search Syntax: Name/Prefix State Zip Word0 Word1 Word2....Wordn\nTo search by word/words only type none 3 times, then your words\nTo Exit: -1\n";
	int max_line_length = 2000;
	char * line = malloc(max_line_length * sizeof(char));
	while(continue_searching == 1)
	{
		getLine(prompt, line, 2000);
		if (strcmp(line, "-1") == 0)
		{
			free(line);
			printf("I really hope this demonstrates my ability to use the C language and problemsolve while coding\n");
			break;
		}
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
		//printf("cell_index: %d\n", cell_index);
		//ensure everything that wasn't entered is null
		switch(cell_index+1)
		{
			case 0:
				free(name);
				name = NULL;
			case 1:
				free(state);
				state = NULL;
			case 2:
				free(zip);
				zip = NULL;
			case 3:
				free(word_array);
				word_array = NULL;
			default:
				break;
		}
/*		printf("name: %s, state: %s, zip: %s, word_array: %p\n", name, state, zip, word_array);
		for (i = 0; i < num_words; ++i)
		{
			printf("%s\n", word_array[i]);
		}
*/
		//using the user entered search, create the business structs
		if (strcmp(name, "none") == 0 && strcmp(state, "none") == 0 && strcmp(zip, "none") == 0)
		{
			noNameYesWords(word_array, num_words, reviews_path);
		}
		else
		{
			struct Business ** businesses;
			int t_businesses = 0;
			businesses = get_business_reviews(bst, name, state, zip, word_array, num_words, &t_businesses, huffman_tree, "dehuffed.txt");
			destroy_business_bst(bst, total_businesses);
			//print out the businesses
			if (t_businesses != 0 && businesses != NULL)
			{
				for (i = 0; i < t_businesses; ++i)
				{
					printBusiness(businesses[i], i, word_array, num_words, huffman_tree);
				}
				destroy_business_result(businesses, t_businesses);
			}
			else
			{
				printf("COULD NOT FIND THE BUSINESS, search again?\n");
			}
			//free the memory
			free(word);
			free(name);
			free(state);
			free(zip);
			for (i = 0; i < num_words; ++i)
			{
				free(word_array[i]);
			}
			free(word_array);
		}
		continue_searching = 0;
	}

	//free memory
	freeHuffmanTree(huffman_tree);

	return EXIT_SUCCESS;
}

void printBusiness(Business * b, int num, char ** words, int num_words, WordData * huffman_tree)
{
	if (b == NULL)
	{
		printf("could not find the business\n");
		return;
	}
	int i;
	printf("=============================================================================================\n");
	printf("Business # %d Name: %s\n", num, b->name);
	printf("Number of Locations: %d\n", b->num_locations);
	if (b->locations == NULL)
	{
		printf("---------------------------------------------------------------------------------------------\n");
		printf("=============================================================================================\n");
		return;
	}
	for (i = 0; i < b->num_locations; i++)
	{
		printLocation(b->locations[i], i, words, num_words, huffman_tree);
	}
	printf("---------------------------------------------------------------------------------------------\n");
	printf("=============================================================================================\n");
	return;
}

void printLocation(Location * l, int num, char ** words, int num_words, WordData * huffman_tree)
{
	printf("---------------------------------------------------------------------------------------------\n");
	printf("Location %d) Address: %s\tState: %s\tCity: %s\tZip Code: %s\n",num, l->address, l->state, l->city, l->zip_code);
	//printReviews("huffed.txt", l, words, num_words, huffman_tree);
	return;
}
