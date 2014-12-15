#include "huffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treefun.h"


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
	array[min] = w;
	for (i = min+1; i < length; i++)
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
				w = WordData_insert(w, word_buffer, &num_unique_words);
				//printf("w->word: %s, w->frequency: %d, w->left: %p, w->right: %p\n",w->word, w->frequency, w->left, w->right);
			}
			//reset the buffer position, add current character to the buffer, and insert word
			Word_buffer_position = 0;
			word_buffer[Word_buffer_position] = (char)current;
			word_buffer[Word_buffer_position+1] = '\0';
			//printf("word inserted: (%s) \n",word_buffer);
			w = WordData_insert(w, word_buffer, &num_unique_words);
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
	//printTree(w);
	printf("num of unique words is: %d\n",num_unique_words);
	
	//create array from BST and sort it by frequency
	int index = 0;
	WordData ** array = (WordData**)malloc(num_unique_words * sizeof(WordData *));
	arrayBuild(array, w, &index, num_unique_words);
	qsort(array, (size_t)num_unique_words, sizeof(WordData *), comparFrequency);
	printf("printing sorted array\n");
	for (i = 0; i < num_unique_words; i++)
	{
		printf("%d) frequency: %d, leaf: %d, word: %s\n",i, array[i]->frequency, array[i]->leaf, array[i]->word );
	}

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
		if (f_smallest_ind == 0)
		{
			s_smallest_ind = 1;
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
		//printf("first smallest(%d)(%d): %s, second smallest(%d)(%d): %s\n",f_smallest_ind, f_smallest->frequency, f_smallest->word, 
		//	s_smallest_ind, s_smallest->frequency, s_smallest->word);
		combined_frequency = f_smallest->frequency + s_smallest->frequency;
		non_leaves[num_non_leaves] = WordData_create(0, "non_leaf", combined_frequency, s_smallest, f_smallest);
		//add that node back into the array, decrement array size 
		arrayInsert(array, non_leaves[num_non_leaves], remaining_nodes, s_smallest_ind, f_smallest_ind);
		remaining_nodes--;
		num_non_leaves++;
	}

	WordData * huffman_tree = array[0];

	printTree(huffman_tree);
	return huffman_tree;

}