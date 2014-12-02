#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "answer08.h"

/**
 * Create a new list-node with the passed string. 
 * str should be copied (with strdup).
 */
List * List_createNode(const char * str){

	List * nd = malloc(sizeof(List));
	nd -> str = strdup(str);
	nd -> next = NULL;

	return nd;
}

List * List_insert(List * head, List * add){
	if (head == NULL)
	{
		return add;
	}
	head -> next = List_insert(head -> next, add);
	return head;
}

/**
 * Free all memory associated with the linked list, including memory for
 * contained strings. Must safely handle NULL lists.
 */
void List_destroy(List * list){
	//the list is null
	if (list == NULL)
	{
		return;
	}
	//the list is not null
	List_destroy(list -> next);
	free(list -> str); //freeing the string
	free(list);
}

/**
 * Length of a linked list. 
 * The length of "NULL" is 0.
 */
int List_length(List * list){

	//while the address of the node isn't null
	int len = 0;
	while(list != NULL){
		//move to next node
		list = list -> next;
		len++;
	}

	return len;
}

void List_print(List * head)
{
   printf ("\nPrint the whole list :\n");
   while ( head != NULL )
   {
      printf ("%s " , head -> str );
      head = head -> next;
   }
   printf ("\n \n");
}

/**
 * Merge two sorted lists to produce a final sorted list.
 * 
 * 'lhs' and 'rhs' are two sorted linked-list. Read 'left-hand-side' and 'right-
 * hand-side'. Note that either (or both) could be NULL, signifying the empty
 * list.
 * 'compar' is a function that is used to compare two nodes. This parameter is
 * similar to the parameter in qsort(...), except that it only compares two
 * strings. You can pass 'strcmp' to this function if you want to sort lists in
 * ascending order.
 *
 * When implementing this function, do not call List_createNode(...) or
 * malloc(...) Instead, think about how to build a new list by:
 * (1) Create a new (empty -- i.e., NULL) list where we build the result. You 
 *     will need to track the first and last node of this list.
 * (2) In a while-loop:
 * (2.a) Use compar to identify whether the front node of lhs or rhs is smaller.
 *       Remember that lhs or rhs could be NULL. 
 * (2.b) Move the front node of 'lhs/rhs' to the end of the result list. 
 *
 * You will need to consider what happens when 'lhs' or 'rhs' becomes NULL.
 *
 * Well-written code should be 20-30 lines long, including comments and spacing.
 * If your code is longer than this, then you may save time by rethinking your
 * approach.
 */
List * List_merge(List * lhs, List * rhs, int (*compar)(const char *, const char*)){

	List * head = NULL;
	List ** current = &head;

    while (lhs && rhs) { //while neither of them are NULL
    	//add from lhs
        if (compar(lhs->str,rhs->str) <= 0) {
            *current = lhs;
            lhs = lhs->next;
        }
        //add from rhs 
        else {
            *current = rhs;
            rhs = rhs->next;
        }
        current = &((*current)->next);
    }
    *current = lhs ? lhs : rhs; // add the last node or nodes
	return head;
}

/** 
 * Sorts the list's elements using the merge-sort algorithm.
 * Merge-sort is a recursive algorithm. See the README for hints.
 * 
 * The brief instructions are as follows:
 *
 * (1) Base case: 
 * Lists of length 0 or 1 are already (defacto) sorted. In this case, return
 * 'list'.
 *
 * (2) Recursive case: 
 * (2.a) Split the linked-list into two approx. equal sized lists.
 * (2.b) Call List_sort(...) on each of these smaller lists.
 * (2.c) Call List_merge(...) to merge the now sorted smaller lists into a 
 *       single larger sorted list, which you return.
 *
 * Well-written code should be 20-30 lines long, including comments and spacing.
 * If your code is longer than this, then you may save time by rethinking your
 * approach.
 */
List * List_sort(List * list, int (*compar)(const char *, const char*)){

	int len = List_length(list);

	//base case-------------------------------------------------------
	if (len < 2)
	{
		return list;
	}

	//recursive case---------------------------------------------------

	//split linked list into two approx. equal sized lists	
	int i;
	int mid;
	List * left = NULL;
	List * right = NULL;
	List ** l_current = &left;
	List ** r_current = &right;

	i = 0;
	mid = len/2;

	while (list){
		//add to left
		if (i < mid)
		{
			*l_current = list; //move the entire list over to left
			list = list -> next; //make list's head the next node of list
			l_current = &(*l_current) -> next; //move to the next node of left
			*l_current = NULL; //ensure that it points to NULL
		}
		//add to right
		else{ 
			*r_current = list;
			list = list -> next;
			r_current = &(*r_current) -> next;

		}
		i++;
	}
	
	//call list sort on each of these smaller lists
	left = List_sort(left, strcmp);
	right = List_sort(right, strcmp);

	//call list merge on these two sorted lists
	List * sorted = NULL;

	sorted = List_merge(left, right, strcmp);

	return sorted;
}


