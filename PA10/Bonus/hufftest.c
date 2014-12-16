#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "oldhuffer.h"

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

	char * reviews_path = "/home/cheif/ece264/solutions/ece264/PA10/reviews.short.tsv";

	int num_words = 0;

	WordData * huffman_tree = parseAndHuff(reviews_path, &num_words);
	printf("num of unique words is: %d\n",num_words);
	
	outputHuffmanFile(huffman_tree, reviews_path, num_words);

	char * huffed_path = "/home/cheif/ece264/solutions/ece264/PA10/Bonus/huffed.txt";

	deHuffer(huffed_path, huffman_tree);

	freeHuffmanTree(huffman_tree);

	return EXIT_SUCCESS;
}