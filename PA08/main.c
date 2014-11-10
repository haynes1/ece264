#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "answer08.h"

int main(int argc, char * * argv)
{
   printf("---------------------Testing Answer08-----------------------\n");
   // Create  two linked lists
   List * head1 = NULL;
   head1 = List_Insert(head1, "a");
   head1 = List_Insert(head1, "b");
   head1 = List_Insert(head1, "c");
   List * head2 = NULL;
   head2 = List_Insert(head2, "d");
   head2 = List_Insert(head2, "e");
   head2 = List_Insert(head2, "f");
   head2 = List_Insert(head2, "g");

   printf("---------------------Testing Merge-----------------------\n");
   List_merge(List * lhs, List * rhs, int (*compar)(const char *, const char*))

}