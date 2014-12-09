#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "answer11.h"

#define MAXLENGTH 1000

HuffNode * HuffNode_create(int value){

	HuffNode * h = malloc(sizeof(HuffNode));
	h->value = value;
	h->left = NULL;
	h->right = NULL;
	return h;
}

void HuffNode_destroy(HuffNode * tree){

	free(tree->left);
	free(tree->right);
	//free(tree->value);
	free(tree);
	return;
}

Stack * Stack_create(){

	Stack * s = malloc(sizeof(Stack));
	s->head = NULL;
	return s;
}

void Stack_destroy(Stack * stack){
	while(stack->head != NULL)
	{
		HuffNode_destroy(stack->head->tree);
		StackNode * next = stack->head->next;
		free(stack->head->next);
		free(stack->head);
		stack->head = next;
	}	
	free(stack);
	return;
}

int Stack_isEmpty(Stack * stack){
	int isempty = 0;
	if (stack->head == NULL)
	{
		isempty = 1;
	}
	return isempty;
}

HuffNode * Stack_popFront(Stack * stack){
	//(1) Save the value (i.e. Huffman tree) of the head node of the stack's list
	HuffNode * h = malloc(sizeof(HuffNode));
	h = stack->head->tree;
	//(2) Remove the head node of the stack's list, freeing it.
	StackNode * next = stack->head->next;
	//HuffNode_destroy(stack->head->tree);
	free(stack->head->next);
	stack->head = next;
	return h;
}

void Stack_pushFront(Stack * stack, HuffNode * tree)
{
	//(1) Create a new StackNode with 'tree' for its tree.
	StackNode * new = malloc(sizeof(StackNode));
	new->tree = tree;
	new->next = stack->head;
	//(2) Push that new StackNode onto the front of the stack's list.
	stack->head = new;
	return;
}

void Stack_popPopCombinePush(Stack * stack){
	//pop first node
	HuffNode * firsthuff = malloc(sizeof(HuffNode));
	firsthuff = Stack_popFront(stack);
	printf("firsthuff value is: %d\n", firsthuff->value);
	//pop second node
	HuffNode * secondhuff = malloc(sizeof(HuffNode));
	secondhuff = Stack_popFront(stack);
	printf("secondhuff value is: %d\n", secondhuff->value);
	//make new hufftree
	HuffNode * tree = HuffNode_create(firsthuff->value + secondhuff->value);
	tree->right = firsthuff;
	tree->left = secondhuff;
	Stack_pushFront(stack, tree);

	return;
}

//Read a Huffman Coding Tree (in text format) from 'fp'.
HuffNode * HuffTree_readTextHeader(FILE * fp){
	int max_nodes = 20;
	int num_nodes = 0;
	HuffNode ** allnodes = malloc(max_nodes * sizeof(HuffNode *));
	HuffNode * final = malloc(sizeof(HuffNode));
	final = NULL;
	Stack * s = Stack_create();
	char * line = malloc(MAXLENGTH * sizeof(char *));
	fgets(line, MAXLENGTH, fp);
	fputs(line, stderr);
	int i = 0;
	while(line[i] != '\n')
	{
		printf("iteration: %d\n", i);
		if (i > max_nodes-1)
		{
			max_nodes *= 2;
			allnodes = realloc(allnodes, max_nodes * sizeof(HuffNode *));
		}
		if (line[i] == '1')
		{
			i++;
			allnodes[num_nodes] = malloc(sizeof(HuffNode));
			allnodes[num_nodes] = HuffNode_create((int)line[i]);
			Stack_pushFront(s, allnodes[num_nodes]);
		}
		if (line[i] == '0')
		{
			StackNode * traverse;
			traverse = s->head;
			while(traverse != NULL)
			{
				traverse = traverse->next;
				num_nodes++;
			}
			printf("number of nodes is: %d\n", num_nodes);
			//test to see if the stack contains only one element
			printf("stack->head->next is: %p\n", s->head->next);
			if (s->head->next == NULL)
			{
				printf("final is here\n");
				final = s->head->tree;
				break;
			}
			//the stack contains more than one element
			Stack_popPopCombinePush(s);
			i++;
		}
		i++;
	}
	Stack_destroy(s);
	return (final);
}

HuffNode * HuffTree_readBinaryHeader(FILE * fp){
	char * line = malloc(MAXLENGTH * sizeof(char));
	fgets(line, MAXLENGTH, fp);
	return NULL;
}