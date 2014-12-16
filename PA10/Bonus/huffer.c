#include "huffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void printTree(WordData * w)
{
	if (w == NULL)
	{
		return;
	}
	printf("word: (%s), frequency: %d, left: %p, right: %p\n", w->word, w->frequency, w->left, w->right);
	printTree(w->left);
	printTree(w->right);
	return;
}

//===================parseAndHuff Helper Functions=============================
WordData * WordData_create(int leaf, char * word, int frequency, struct WordData * left,  struct WordData * right)
{
	WordData * w = malloc(sizeof(WordData));
	w->code = NULL;
	w->leaf = leaf;
	w->word = strdup(word);
	w->frequency = frequency;
	w->left = left;
	w->right = right;
	return w;
}

/*
calls WordData_create, and as such that function will appear nowhere else
handles incrementing word frequency when 
*/
WordData * WordData_insert(WordData * w, char * word, int * num_unique_words)
{
	//w is empty, create node
	if (w == NULL)
	{
		//printf("making new node: %s\n", word );
		int uwords = *num_unique_words;
		uwords++;
		*num_unique_words = uwords;
		return WordData_create(1, word, 1, NULL, NULL);
	}
	//the words are the same, increment frequency
	if (strcmp(word, w->word) == 0)
	{
		w->frequency++;
	}
	//the words are not the same
	//go left
	if (strcmp(word, w->word) < 0)
	{
		w->left = WordData_insert(w->left, word, num_unique_words);
	}
	//go right
	if (strcmp(word, w->word) > 0)
	{
		w->right = WordData_insert(w->right, word, num_unique_words);
	}
	return w;
}

int numNodes(WordData *n)
{
    int c = 1;
 
    if (n == NULL)
    {
        return 0;
    }
    else
    {
        c += numNodes(n->left);
        c += numNodes(n->right);
    }
    return c;
}

void arrayBuild(WordData ** array, WordData *w, int * index, int length)
{
	int ind = *index;
	if (ind >= length || w == NULL)
	{
		return;
	}
	//printf("ind: %d, word: (%s)\n", ind, w->word);
	array[ind] = w;
	ind++;
	*index = ind;
	arrayBuild(array, w->left, index,length);
	arrayBuild(array, w->right, index, length);
	return;
}

int comparFrequency(const void * a, const void * b)
{
	const struct WordData *l1 = *(WordData **)a;
	const struct WordData *l2 = *(WordData **)b;
	int comparison;
	if (l1->frequency < l2->frequency)
	{
		comparison = 1;
	}
	else if (l1->frequency == l2->frequency)
	{
		comparison = 0;
	}
	else if (l1->frequency > l2->frequency)
	{
		comparison = -1;
	}

	return comparison;
}

int findSmallest(WordData ** array, int length)
{
	int i = length-1;
	int last_frequency = array[length]->frequency;
	if (last_frequency < array[i]->frequency)
	{
		return length;
	}
	else if (last_frequency == array[i]->frequency)
	{
		//printf("there are more than one in a row from last\n");
		while(array[i]->frequency == last_frequency)
		{
			i--;
		}
		//printf("last_frequency = %d, array[%d]->frequency = %d\n", last_frequency, i, array[i]->frequency);
		if (array[i]->frequency > last_frequency)
		{
			//printf("	it will be the last: %d\n", length);
			return length;
		}
		else
		{
			//printf("	it will be current: %d\n", i);
			return i;
		}
	}
	else if (last_frequency > array[i]->frequency)
	{
		return i;
	}
	return -1;
}

void arrayInsert(WordData ** array, WordData * w, int length, int f_ind, int s_ind)
{
	int i;
	int min = f_ind < s_ind ? f_ind: s_ind;
	int max = f_ind < s_ind ? s_ind: f_ind;
	array[min] = w;
	for (i = max; i < length; i++)
	{
		array[i] = array[i+1];
	}
	//printf("print the array, first index: %d, second index: %d\n",f_ind, s_ind);
	for (i = 0; i < length; i++)
	{
		//printf("%d)frequency: %d, leaf: %d, word: %s" ,i,  array[i]->frequency, array[i]->leaf, array[i]->word);
		if (array[i]->leaf == 0)
		{
			//printf(":left->word is (%s), right->word is (%s)", array[i]->left->word, array[i]->right->word);
		}
		//printf("\n");
	}
	//free(array[length]);
	return;
}

//=========================End of Helper Functions===============================

WordData * parseAndHuff(char * reviews_path, int * word_count)
{
	int i = 0;
	FILE * reviews_stream = fopen(reviews_path, "r");
	if (reviews_stream == NULL)
	{
		printf("reviews stream failed to open\n");
		return NULL;
	}

	//create BST of WordData structs by name
	int num_unique_words = 0;
	int word_buffer_position = 0;
	int max_word_size = 200;
	WordData * w = NULL;
	char * word_buffer = malloc(max_word_size * sizeof(char));
	while(!feof(reviews_stream))
	{
		int current = fgetc(reviews_stream);
		//fputc((char)current, stderr);
		//Test: is current a ' ', '\t' or '\n' (word separators): insert the buffer, then insert the current character
		if (current == ' ' || current == '\t' || current == '\n' || current == '.' || current == ',')
		{
			//insert word up untill current character unless the last word was a word separator
			if (word_buffer[0] != ' ' && word_buffer[0] != '\t' && word_buffer[0] != '\n' 
				&& word_buffer[0] != '.' && word_buffer[0] != ',')
			{
				//printf("word inserted: (%s) \tcurent character is: (%c)\n",word_buffer, current);
				w = WordData_insert(w, word_buffer, &num_unique_words);
				//printf("w->word: %s, w->frequency: %d, w->left: %p, w->right: %p\n",w->word, w->frequency, w->left, w->right);
			}
			//reset the buffer position, add current character to the buffer, and insert word
			word_buffer_position = 0;
			word_buffer[word_buffer_position] = (char)current;
			word_buffer[word_buffer_position+1] = '\0';
			//printf("word inserted: (%s) \n",word_buffer);
			w = WordData_insert(w, word_buffer, &num_unique_words);
		}
		else
		{
			word_buffer[word_buffer_position] = (char)current;
			word_buffer[word_buffer_position+1] = '\0';
			word_buffer_position++;
		}
		i++;
	}
	free(word_buffer);
	fclose(reviews_stream);
	//printTree(w);
	*word_count = num_unique_words;
	//printf("num of unique words is: %d\n",num_unique_words);
	
	//create array from BST and sort it by frequency
	int index = 0;
	WordData ** array = (WordData**)malloc(num_unique_words * sizeof(WordData *));
	arrayBuild(array, w, &index, num_unique_words);
	qsort(array, (size_t)num_unique_words, sizeof(WordData *), comparFrequency);
	//printf("printing sorted array\n");
	/*for (i = 0; i < num_unique_words; i++)
	{
		printf("%d) frequency: %d, leaf: %d, word: (%s)\n",i, array[i]->frequency, array[i]->leaf, array[i]->word );
	}*/
	//printf("number of unique words is: %d\n", num_unique_words);
	//build the huffman tree
	int max_non_leaves = 1000;
	int num_non_leaves = 0;
	WordData ** non_leaves = malloc(max_non_leaves * sizeof(WordData *));
	int remaining_nodes = num_unique_words-1;
	int combined_frequency;
	int f_smallest_ind;
	int s_smallest_ind;
	while(remaining_nodes >= 1)
	{
		//ensure there is enough space in the array of non leaves
		if (num_non_leaves >= max_non_leaves-1)
		{
			max_non_leaves *= 2;
			non_leaves = realloc(non_leaves, max_non_leaves * sizeof(WordData *));
		}
		//take 2 lowest frequency nodes, combine them
		f_smallest_ind = findSmallest(array, remaining_nodes);
		if (remaining_nodes == 1)
		{
			s_smallest_ind = f_smallest_ind? 0: 1;
		}
		else if (f_smallest_ind != remaining_nodes && array[f_smallest_ind-1]->frequency > array[remaining_nodes]->frequency)
		{
			s_smallest_ind = remaining_nodes;
		}
		else
		{
			s_smallest_ind = f_smallest_ind-1;
		}
		WordData * f_smallest = array[f_smallest_ind];
		WordData * s_smallest = array[s_smallest_ind];
		//ensure all leaf's children point to null
		if (f_smallest->leaf == 1)
		{
			f_smallest->left = NULL;
			f_smallest->right = NULL;
		}
		if (s_smallest->leaf == 1)
		{
			s_smallest->left = NULL;
			s_smallest->right = NULL;
		}
		//strcat(f_smallest->word,s_smallest->word)
		//printf("first smallest(%d)(%d): %s, second smallest(%d)(%d): %s\n",f_smallest_ind, f_smallest->frequency, f_smallest->word, 
		//	s_smallest_ind, s_smallest->frequency, s_smallest->word);
		combined_frequency = f_smallest->frequency + s_smallest->frequency;
		non_leaves[num_non_leaves] = WordData_create(0, "non leaf" , combined_frequency, f_smallest, s_smallest);
		//add that node back into the array, decrement array size 
		arrayInsert(array, non_leaves[num_non_leaves], remaining_nodes, s_smallest_ind, f_smallest_ind);
		remaining_nodes--;
		num_non_leaves++;
	}

	WordData * huffman_tree = array[0];
	free(array);
	free(non_leaves);
	//printTree(huffman_tree);
	return huffman_tree;
}

//========================outputHuffmanFile Helper Functions=====================
WordCode * WordCode_create(char* word, char * code)
{
	WordCode * w = malloc(sizeof(WordCode));
	w->word = strdup(word);
	w->code = code;
	return w;
}

char * nextCode(char * current_code, int code_length, char direction)
{
	char * next_code = strdup(current_code);
	next_code = realloc(next_code, (code_length+2) * sizeof(char));
	next_code[code_length] =  direction;
	next_code[code_length+1] = '\0';
	//printf("code (%s), direction: %c nextcode (%s), next_length = %d\n", current_code, direction, next_code, code_length);
	return next_code;
}

void moveThroughTree(WordCode ** array, WordData * n, char * code, int code_length, int *num_words, int * max)
{
	n->code = code;
	if (n->leaf == 1)
	{
		int num = *num_words;
		int t_max = *max;
		//ensure there is enough room in the array of WordCodes
		if (num >= t_max-1)
		{
			t_max *= 2;
			void * tmp = realloc(array, t_max * sizeof(WordCode *));
			if (tmp != NULL)
			{
				array = tmp;
			}
			else
			{
				printf("failed to realloc, everything will die\n");
			}
			*max = t_max;
		}
		array[num] = WordCode_create(n->word, code);
		num++;
		*num_words = num;
		return;
	}
	else
	{
		moveThroughTree(array, n->left, nextCode(code, code_length, '0'), code_length+1, num_words, max);
		moveThroughTree(array, n->right, nextCode(code, code_length, '1'), code_length+1, num_words, max);
		//free(code);
	}
	return;
}

int comparCodes(const void * a, const void * b)
{
	const struct WordCode *l1 = *(WordCode **)a;
	const struct WordCode *l2 = *(WordCode **)b;

	return(strcmp(l1->word, l2->word));
}

char * itoa(int value){
	if (value == 0)
	{
		return strdup("0");
	}
	int i = 0;
	char * str = malloc(40 * sizeof(char));
	while(value != 0)
	{		
		str[i] = (value % 10) + 48;
		value /= 10;
		i++;
	}
	str[i] = '\0';
	//reverse the string
	int c = strlen(str);
	char * rev = malloc(c + 1 * sizeof(char));
	c--;
	for (i = 0; i < strlen(str); i++)
	{
		rev[i] = str[c];
		c--;
	}
	rev[i] = '\0';
	free(str);
	return rev;
}

char * searchCode(char * word, WordCode ** array, int start, int end)
{
	char * code;
	int mid = (start + end) / 2;
	char * mid_word = array[mid]->word;
	if (start > end)
	{
		//printf("couldn't find word\n");
		return "error";
	}
	//found the word
	if (strcmp(word, mid_word) == 0)
	{
		code = array[mid]->code;
	}
	//go left
	else if (strcmp(word, mid_word) < 0)
	{
		code = searchCode(word, array, start, mid-1);
	}
	//go right
	else if (strcmp(word, mid_word) > 0)
	{
		code = searchCode(word, array, mid+1, end);
	}

	return code;
}

WordCode ** createCodeArray(WordData * huffman_tree, int word_count)
{
	//make array of WordCodes
	int num = 0;
	int max_word_codes = word_count + 1;
	WordCode ** code_array = malloc(max_word_codes * sizeof(WordCode *));
	/*char * zero = malloc(2 * sizeof(char));
	zero[0] = '0';
	zero[1] = '\0';
	char * one = malloc(2 * sizeof(char));
	one[0] = '1';
	one[1] = '\0';*/
	char * nothing = malloc(sizeof(char));
	nothing[0] = '\0';
	moveThroughTree(code_array, huffman_tree, nothing, 0, &num, &max_word_codes);
	//moveThroughTree(code_array, huffman_tree->right, one, 1, &num, &max_word_codes);
	printf("num words found in in createCodeArray = %d\n", num);
	qsort(code_array, (size_t)word_count, sizeof(WordCode *), comparCodes);
	return code_array;
}

void freeCodeArray(WordCode** array, int length)
{
	int i;
	for (i = 0; i < length; i++)
	{
		free(array[i]->word);
		//free(array[i]->code);
		free(array[i]);
	}
	free(array);
	return;
}


//=============================END Helper Functions==============================

void outputHuffmanFile(WordData * huffman_tree, char * reviews_path, int word_count)
{
	int i;
	WordCode ** code_array = createCodeArray(huffman_tree, word_count);
	/*printf("testing the wordCode array: contains %d words\n", num_words);
	for (i = 0; i < num_words; i++)
	{
		printf("%d) word: (%s) code: %s\n", i, code_array[i]->word, code_array[i]->code);
	}*/
	//printTree(huffman_tree);

	//using the array of WordCodes output to a file
	FILE * reviews_stream = fopen(reviews_path, "r");
	FILE * output = fopen("huffed.txt", "w");
	int word_buffer_position = 0;
	int max_word_size = 50;
	char * word_buffer = malloc(max_word_size * sizeof(char));
	char * output_code;
	while(!feof(reviews_stream))
	{
		int current = fgetc(reviews_stream);
		//Test: is current a ' ', '\t' or '\n' (word separators): insert the buffer, then insert the current character
		if (current == ' ' || current == '\t' || current == '\n' || current == '.' || current == ',' || current == '(' || current == ')')
		{
			//insert word up untill current character unless the last word was a word separator
			if (word_buffer[0] != ' ' && word_buffer[0] != '\t' && word_buffer[0] != '\n' 
				&& word_buffer[0] != '.' && word_buffer[0] != ',' && word_buffer[0] != '(' && word_buffer[0] != ')')
			{
				output_code = searchCode(word_buffer, code_array, 0, word_count-1);
				fputs(output_code, output);
				//printf("code written: (%s)\n", s_output_code);
			}
			//reset the buffer position, add current character to the buffer, and insert word
			word_buffer_position = 0;
			word_buffer[word_buffer_position] = (char)current;
			word_buffer[word_buffer_position+1] = '\0';
			output_code = searchCode(word_buffer, code_array, 0, word_count-1);
			fputs(output_code, output);
			//printf("code written: (%s)\n", s_output_code);
		}
		else
		{
			word_buffer[word_buffer_position] = (char)current;
			word_buffer[word_buffer_position+1] = '\0';
			word_buffer_position++;
		}
		i++;
	}
	free(word_buffer);
	fclose(reviews_stream);
	fclose(output);
	freeCodeArray(code_array, word_count);

	return;
}

//=============================deHuffer Helper Functions==============================
int testLeafAndOutput(WordData * current_node, FILE * output)
{
	int leaf;
	//current node is not a leaf node
	if (current_node->leaf == 0)
	{
		leaf = 0;
	}
	//current node is a leaf node
	else if (current_node->leaf == 1)
	{
		leaf = 1;
		//printf("output) leaf: %d, word: %s, left; %p, right: %p\n",current_node->leaf, current_node->word, current_node->left, current_node->right);
		fputs(current_node->word, output);
	}
	return leaf;
}

//=================================END Helper Functions================================
void deHuffer(char * huffed_path, WordData * huffman_tree)
{
	//printTree(huffman_tree);
	int i = 0;
	FILE * huffed = fopen(huffed_path, "r");
	if (huffed == NULL)
	{
		printf("failed to open the huffman encoded file\n");
	}
	FILE * dehuffed = fopen("dehuffed.txt", "w");
	WordData * traverse = huffman_tree;
	int is_leaf;
	while(!feof(huffed))
	{
		int current = fgetc(huffed);
		//fputc(current, stderr);
		if (current == '0')
		{
			//printf("read a 0\n");
			traverse = traverse->left;
			is_leaf = testLeafAndOutput(traverse, dehuffed);
		}
		else if (current == '1')
		{
			//printf("read a 1\n");
			traverse = traverse->right;
			is_leaf = testLeafAndOutput(traverse, dehuffed);
		}
		//if a leaf was found return to the top of the huffman tree
		if (is_leaf == 1)
		{
			traverse = huffman_tree;
		}
		i++;
	}
	fclose(huffed);
	fclose(dehuffed);
	return;
}

//=================================DESTRUCTION Functions================================

void freeHuffmanTree(WordData * t)
{
	if (t == NULL)
	{
		return;
	}
	free(t->word);
	free(t->code);
	freeHuffmanTree(t->left);
	freeHuffmanTree(t->right);
	free(t);

	return;
}