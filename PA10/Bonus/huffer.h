#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//the struct that will makeup both the searchable BST and the Huffman Tree

typedef struct WordData{
	int leaf; //will be 1 if the node is a leaf, 0 otherwise
	char * word; //the word itself
	int frequency; //the number of occurances of the word
	char * code; //the word's code
	char * leftstring; //string of left chilren's words
	char * rightstring; //string of right children's words
	struct WordData * left; //pointer to left child
	struct WordData * right; //pointer to right child
}WordData;

typedef struct WordCode{
	char * word;
	char * code;
}WordCode;

//returns a huffman tree of words 
WordData * parseAndHuff(char* reviews_path, int * word_count);

void outputHuffmanFile(WordData * huffman_tree, char * reviews_path, int word_count);

void deHuffer(char * huffed_path, WordData * huffman_tree);

void freeHuffmanTree(WordData * t);

