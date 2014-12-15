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

	char * reviews_path = "/home/cheif/ece264/solutions/ece264/PA10/Bonus/test.txt";

	WordData * huffman_tree = parseAndHuff(reviews_path);
	
	outputHuffmanFile(huffman_tree, reviews_path);

	char * huffed_path = "/home/cheif/ece264/solutions/ece264/PA10/Bonus/huffed.txt";

	deHuffer(huffed_path, huffman_tree);

	return EXIT_SUCCESS;
}