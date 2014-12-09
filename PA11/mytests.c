#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "answer11.h"
#include "treefun.c"

int main(int argc, char * * argv)
{
	char * input_file_path = "/home/pharaoh/ece264/solutions/ece264/PA11/inputs/01-tibbs.txt-huff";
	FILE * input_file = fopen(input_file_path, "r");
	HuffNode * tree;
	tree = HuffTree_readTextHeader(input_file);

	HuffNode_printPretty(stdout, tree);

	return EXIT_SUCCESS;
}