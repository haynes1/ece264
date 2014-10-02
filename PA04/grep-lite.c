#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define SIZE 2000

int main(int argc, char * * argv)
{
   // We want some variables to store which "switches" were set
   int showHelp = FALSE;
   int V_OP = FALSE;
   int N_OP = FALSE;
   int Q_OP = FALSE;
   int ERR = FALSE;
   char* PATTERN;

   if ((char)(*argv[argc]) == '-')
   {
      ERR = TRUE;
   }
   else{
      PATTERN = argv[argc];
   }


   // Loop through the arguments, looking for our switches...
   int ind = 1; // we always skip 0, which is the program path
   for( ; ind < argc; ++ind) {
      if(strcmp(argv[ind], "--help") == 0) 
         showHelp = TRUE;
      else if(strcmp(argv[ind], "-v") == 0)
         V_OP = TRUE;
      else if(strcmp(argv[ind], "-i") == 0)
         N_OP = TRUE;
      else if(strcmp(argv[ind], "-i") == 0)
         Q_OP = TRUE;
      else {
         ERR = TRUE; // best not to run if there's an error
      }
   }

   if(ERR) {
      fprintf(stderr, "Error\n");
      return EXIT_FAILURE;
   }

   // If --help is passed, you usually show help and do nothing else
   if(showHelp) {
      printf("Usage: grep-lite [OPTION]... PATTERN\n"
      "Search for PATTERN in standard input. PATTERN is a\n"
      "string. grep-lite will search standard input line by\n"
      "line, and (by default) print out those lines which\n"
      "contain pattern as a substring.\n\n\n"
      "  -v, --invert-match     print non-matching lines\n"
      "  -n, --line-number      print line numbers with output\n"
      "  -q, --quiet            suppress all output\n\n"
      "Exit status is 0 if any line is selected, 1 otherwise;\n"
      "if any error occurs, then the exit status is 2.");
      return EXIT_SUCCESS;
   }

   char * buffer = malloc(SIZE * sizeof(char));
   if (buffer != NULL)
   {
      while(fgetc(stdin) != EOF){//go till end of file
         while(fgetc(stdin) != \n)//go till a new line
      }
   }


   return EXIT_SUCCESS;
}
