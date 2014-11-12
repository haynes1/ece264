#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "answer08.h"

List * List_Insert(List * head , char * val);
void List_print(List * head);

int main(int argc, char * * argv)
{
   printf("---------------------Testing Answer08-----------------------\n");
   // Create  two linked lists
   List * head1 = NULL;
   head1 = List_Insert(head1, "a");
   head1 = List_Insert(head1, "c");
   head1 = List_Insert(head1, "e");
   List_print(head1);
   List * head2 = NULL;
   head2 = List_Insert(head2, "b");
   head2 = List_Insert(head2, "d");
   head2 = List_Insert(head2, "f");
   head2 = List_Insert(head2, "g");
   List_print(head2);
   printf("---------------------Testing Merge-----------------------\n");
   List * newlist;
   newlist = List_merge(head1, head2, strcmp);

   return EXIT_SUCCESS;

}
List * List_Insert(List * head, char * str){
   if (head == NULL)
   {
      return List_createNode(str);
   }
   head -> next = List_Insert(head -> next, str);
   return head;
}
