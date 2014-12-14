#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffer.h"

/*void printTree(WordData * w)
{
	if (w == NULL)
	{
		return;
	}
	printTree(w->left);
	printf("word: %s, frequency: %d\n", w->word, w->frequency);
	printTree(w->right);
	return;
}*/


int main(int argc, char *argv[]){

	char * reviews_path = "/home/pharaoh/ece264/solutions/ece264/PA10/Bonus/test.txt";

	WordData * huff_tree = parseAndHuff(reviews_path);
	//printTree(huff_tree);

	return EXIT_SUCCESS;
}