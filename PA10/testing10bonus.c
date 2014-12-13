#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <string.h>
#include "answer10bonus.h"

int main(int argc, char *argv[]){
	printf("====================== Testing Create BST ====================\n");
	char * businesses_path = "/home/cheif/ece264/solutions/ece264/PA10/businesses.short.tsv";
	char * reviews_path = "/home/cheif/ece264/solutions/ece264/PA10/reviews.short.tsv";
	struct YelpDataBST * bst = NULL;
	bst = create_business_bst(businesses_path, reviews_path);
	printf("testing search by prefix\n");
	//int num_businesses = 0;
	/*BusinessTree * tree = bst->business_tree;
	BusinessTree ** prefix_search_results = searchTreee(tree, "Star", &num_businesses);
	for (i = 0; i < num_businesses; i++)
	{
		printf("name is: %s\n", prefix_search_results[i]->name);
	}*/

	printf("====================== Testing The Search ====================\n");
	struct Business ** b;
	char ** words = malloc(4 * sizeof(char *));
	words[0] = "vanilla";
	words[1] = "frap";
	words[2] = "with";
	words[3] = "banana";
	int num_businesses = 0;
	b = get_business_reviews(bst, "Starbucks", NULL, NULL, words, 4, &num_businesses);
	destroy_business_bst(bst);
	destroy_business_result(b, num_businesses);
	
	return EXIT_SUCCESS;
}

/*strategy for fixing the whole treearray memory thing
	have the YelpDataBST include the index of the root of treearray
	this way I can just pass treearray as the the attribute business_tree
	this enables me to actually free it later, and makes freeing it easier
*/