
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * * argv)
{
   int i; // we always skip 0, which is the program path
   for( i = 1; i < argc; i++) {

      printf("%s ", argv[i]);
   }
   printf("\n");
   return EXIT_SUCCESS;
}
