#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//the struct that will makeup both the searchable BST and the Huffman Tree

typedef struct WordData{
	int leaf; //will be 1 if the node is a leaf, 0 otherwise
	char * word; //the word itself
	int frequency; //the number of occurances of the word
	struct WordData * left; //pointer to left child
	struct WordData * right; //pointer to right child
} WordData;

//returns a huffman tree of words 
WordData * parseAndHuff(char * reviews_path);
