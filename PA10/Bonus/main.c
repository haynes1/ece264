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

	//prompt user, and do the searches
	int continue_searching = 1;
	char * prompt = "Search Syntax: Name/Prefix State Zip Word0 Word1 Word2....Wordn\n";
	int max_line_length = 2000;
	char * line = malloc(max_line_length * sizeof(char));
	getLine(prompt, line, 2000);
	printf("search for: %s\n", line);
	//explode the search string
	char name[MAXNAME];
	char state[MAXSTATE];
	char zip[MAXZIP];
	char word = malloc(MAXWORD * sizeof(char));
	int max_words_in_array;
	char ** word_array = malloc(max_words_in_array * sizeof(char *));
	int b = 0;
	int a = 0;
	int cell_index = 0;
	while(line[a] != '\0')
	{
		if (line[a] == ' ') //test to see if we've moved to a new cell
		{
			cell_index++;
			b = 0;
			a++; //to move past the '\t'
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
		//adding words to the word array
		if (cell_index > 2)
		{
			
		}

		a++;
	}

	return EXIT_SUCCESS;
}