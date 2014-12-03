#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "answer10.h"

int main(int argc, char *argv[]){

	printf("====================== Testing Create BST ====================\n");
	char * businesses_path = "/home/pharaoh/ece264/solutions/ece264/PA10/businesses.tsv";
	char * reviews_path = "/home/pharaoh/ece264/solutions/ece264/PA10/reviews.tsv";
	struct YelpDataBST * bst = NULL;
	bst = create_business_bst(businesses_path, reviews_path);
	printf("%p\n", bst);

	printf("====================== Testing The Search ====================\n");
	get_business_reviews(bst, "Starbucks", NULL, NULL);


	return EXIT_SUCCESS;
}