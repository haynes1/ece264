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
	//printf("word: (%s), frequency: %d, left: %p, right: %p\n", w->word, w->frequency, w->left, w->right);
	printTree(w->right);
	return;
}

//===================parseAndHuff Helper Functions=============================
WordData * WordData_create(int leaf, char * word, struct WordData * left,  struct WordData * right)
{
	WordData * w = malloc(sizeof(WordData));
	w->leaf = leaf;
	w->word = strdup(word);
	w->frequency = 1;
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
		//printf("making new node: %s\n", word );
		int uwords = *num_unique_words;
		uwords++;
		*num_unique_words = uwords;
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
				w = WordData_insert(w, 0, word_buffer, &num_unique_words);
				//printf("w->word: %s, w->frequency: %d, w->left: %p, w->right: %p\n",w->word, w->frequency, w->left, w->right);
			}
			//reset the buffer position, add current character to the buffer, and insert word
			Word_buffer_position = 0;
			word_buffer[Word_buffer_position] = (char)current;
			word_buffer[Word_buffer_position+1] = '\0';
			//printf("word inserted: (%s) \n",word_buffer);
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
	free(word_buffer);
	fclose(reviews_stream);
	printTree(w);
	printf("num of unique words is: %d\n",num_unique_words);
	
	//create array from BST and sort it by frequency
	int index = 0;
	WordData ** array = (WordData**)malloc(num_unique_words * sizeof(WordData *));
	arrayBuild(array, w, &index, num_unique_words);
	qsort(array, (size_t)num_unique_words, sizeof(WordData *), comparFrequency);
/*
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

	//WordData * huffman_tree;// = array[remaining_nodes];
*/
	return NULL;

}