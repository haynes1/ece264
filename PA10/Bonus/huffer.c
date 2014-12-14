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
	printTree(w->left);
	printf("word: %s, frequency: %d\n", w->word, w->frequency);
	printTree(w->right);
	return;
}

//===================parseAndHuff Helper Functions=============================
WordData * WordData_create(int leaf, char * word, struct WordData * left,  struct WordData * right)
{
	WordData * w = malloc(sizeof(WordData));
	w->leaf = leaf;
	w->word = strdup(word);
	w->frequency = 0;
	w->left = left;
	w->right = right;
	return w;
}

/*
calls WordData_create, and as such that function will appear nowhere else
handles incrementing word frequency when 
*/
WordData * WordData_insert(WordData * w, int leaf, char * word, int * num_unique_words)
{
	//w is empty, create node
	if (w == NULL)
	{
		*num_unique_words++;
		return WordData_create(leaf, word, NULL, NULL);
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
		w->left = WordData_insert(w->left, leaf, word, num_unique_words);
	}
	//go right
	if (strcmp(word, w->word) > 0)
	{
		w->right = WordData_insert(w->right, leaf, word, num_unique_words);
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
	if (*index >= length)
	{
		return;
	}
	array[*index] = w;
	*index++;
	arrayBuild(array, w->left, index,length);
	arrayBuild(array, w->right, index, length);
	return;
}

int comparFrequency(const void * a, const void * b)
{
	const struct WordData *p = (WordData *)a;
	const struct WordData *q = (WordData*)b;

	if (p->frequency > q->frequency)
	{
		return -1;
	}
	if (p->frequency == q->frequency)
	{
		return 0;
	}
	if (p->frequency < q->frequency)
	{
		return 1;
	}
	return 0;
}

//=========================End of Helper Functions===============================

WordData * parseAndHuff(char * reviews_path)
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
	int Word_buffer_position = 0;
	int max_word_size = 50;
	WordData * w;
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
				printf("word inserted: (%s) \tcurent character is: (%c)\n",word_buffer, current);
				w = WordData_insert(w, 0, word_buffer, &num_unique_words);
			}
			//reset the buffer position, add current character to the buffer, and insert word
			Word_buffer_position = 0;
			word_buffer[Word_buffer_position] = (char)current;
			word_buffer[Word_buffer_position+1] = '\0';
			printf("word inserted: (%s) \n",word_buffer);
			w = WordData_insert(w, 0, word_buffer, &num_unique_words);
		}
		else
		{
			word_buffer[Word_buffer_position] = (char)current;
			word_buffer[Word_buffer_position+1] = '\0';
			Word_buffer_position++;
		}
		i++;
	}
	free(reviews_stream);
	printf("num of unique words is: %d\n",num_unique_words);
	//printTree(w);
	/*
	//create array from BST and sort it by frequency
	int total_words = numNodes(w);
	int index = 0;
	WordData ** array = NULL;
	arrayBuild(array, w, &index, total_words);
	qsort(array, (size_t)total_words, sizeof(WordData *), comparFrequency);

	//build the huffman tree
	int remaining_nodes = total_words;
	while(remaining_nodes >= 1)
	{
		if (array[remaining_nodes] < array[remaining_nodes-1]) //the tree node is still the smallest node
		{
			WordData * f_smallest = array[remaining_nodes];
			WordData * s_smallest = array[remaining_nodes];
			WordData * non_leaf = WordData_create(0, NULL, s_smallest, f_smallest);
			remaining_nodes--;
			array[remaining_nodes] = non_leaf;
		}
		else
		{
			WordData * f_smallest = array[remaining_nodes-1];
			WordData * s_smallest = array[remaining_nodes-2];
			WordData * non_leaf = WordData_create(0, NULL, s_smallest, f_smallest);
			remaining_nodes--;
			array[remaining_nodes-1] = non_leaf;

		}
	}
*/
	//WordData * huffman_tree;// = array[remaining_nodes];

	return NULL;

}