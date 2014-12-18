#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "answer10bonus.h"

#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2
#define MAXNAME 500
#define MAXSTATE 6
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

	char businesses_path[] = "/home/pharaoh/ece264/solutions/ece264/PA10/businesses.tsv";
	char reviews_path[] = "/home/pharaoh/ece264/solutions/ece264/PA10/reviews.tsv";

	if(argc == 1) {
		fprintf(stderr, "=================================================Starting Program=============================================\n");
		fprintf(stderr, "Run this program with a 1 as the argument to procede.\nThis will start the building of the BST\n");
		fprintf(stderr, "It will take 1ish min to complete, and will only happen once before multiple searches can be made.\n\n");
		return EXIT_FAILURE;
	}
	if (argc == 2 && strcmp(argv[1], "1") == 0)
	{
		fprintf(stderr, "=================================================Starting Program=============================================\n");
		fprintf(stderr, "Starting to build the BST.\n");
		fprintf(stderr, "This will take approximately 1ish minutes to complete before you will be prompted for multiple searches\n");
	}
	else
	{
		printf("argv[1] must be 1 to continue.\n");
		return EXIT_FAILURE;
	}

	//create the BST
	struct YelpDataBST * bst = NULL;
	int total_businesses = 0;
	bst = create_business_bst(businesses_path, reviews_path, &total_businesses);

	//prompt user, and do the searches
	int continue_searching = 1;
	char * prompt1 = "Name: ";
	char * promptstate = "State: ";
	char * promptzip = "Zip: ";
	char * promptwords ="Words: ";
	int max_line_length = 2000;
	char * line = malloc(max_line_length * sizeof(char));
	while(continue_searching == 1)
	{
		printf("++++++\nStarting a Search\nEnter -1 at any time to exit\n+++++\n");
		char * name = malloc(MAXNAME * sizeof(char));
		char * state = malloc(MAXSTATE * sizeof(char));
		char * zip = malloc(MAXZIP * sizeof(char));
		char * word = malloc(MAXWORD * sizeof(char));
		int max_words_in_array = 20;
		char ** word_array = malloc(max_words_in_array * sizeof(char *));
		getLine(prompt1, name, MAXNAME);
		if (strcmp(name, "-1") == 0)
		{
			printf("I really hope this demonstrates my ability to use the C language and problemsolve while coding\n");
			free(name);
			break;
		}
		getLine(promptstate, state, MAXSTATE);
		if (strcmp(state, "-1") == 0)
		{
			printf("I really hope this demonstrates my ability to use the C language and problemsolve while coding\n");
			free(name);
			free(state);
			break;
		}
		getLine(promptzip, zip, MAXZIP);
		if (strcmp(zip, "-1") == 0)
		{
			printf("I really hope this demonstrates my ability to use the C language and problemsolve while coding\n");
			free(name);
			free(state);
			free(zip);
			break;
		}
		getLine(promptwords, line, 2000);
		if (strcmp(line, "-1") == 0)
		{
			printf("I really hope this demonstrates my ability to use the C language and problemsolve while coding\n");
			free(name);
			free(state);
			free(zip);
			free(word_array);
			free(line);
			break;
		}
		//take in the words to a string array
		int b = 0;
		int a = 0;
		int num_words = 0;
		while(line[a] != '\0')
		{
			if (line[a] == ' ') //test to see if we've moved to a new cell
			{
				b = 0;
				a++; //to move past the ' '
			}
			word[b] = line[a];
			b++;
			word[b] = '\0';
			//found an entire word, add it to the array
			if (line[a+1] == ' ' || line[a+1] == '\0' || line[a+1] == '\n')
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
			a++;
		}
		if (strlen(name) == 0)
		{
			name = NULL;
		}
		if (strlen(state)==0)
		{
			state = NULL;
		}
		if (strlen(zip) == 0)
		{
			zip = NULL;
		}
		printf("words:\n");
		for (i = 0; i < num_words; ++i)
		{
			printf("%s\n",word_array[i] );
		}
		printf("searched for: name: %s, state: %s, zip: %s, num words: %d\n",name ,state,zip, num_words );
		//using the user entered search, create the business structs
		if (name == NULL && state == NULL && zip == NULL && num_words != 0)
		{
			noNameYesWords(word_array, num_words, reviews_path);
		}
		else
		{
			struct Business ** businesses;
			int t_businesses = 0;
			businesses = get_business_reviews(bst, name, state, zip, word_array, num_words, &t_businesses);

			//print out the businesses
			if (t_businesses != 0)
			{
				for (i = 0; i < t_businesses; ++i)
				{
					printBusiness(businesses[i], i);
				}
			}
			else
			{
				printf("COULD NOT FIND THE BUSINESS\n");
			}
			destroy_business_result(businesses, t_businesses);
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

	//free memory
	destroy_business_bst(bst, total_businesses);

	return EXIT_SUCCESS;
}

void printBusiness(Business * b, int num)
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