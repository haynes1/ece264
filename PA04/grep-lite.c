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
   char * PATTERN;
   int exit_status = 1;


   // Loop through the arguments, looking for switches
   int i; // we always skip 0, which is the program path
   for(i = 1 ; i < argc; ++i) {
      if(strcmp(argv[i], "--help") == 0) 
         showHelp = TRUE;
      else if(strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--invert-match") == 0)
         V_OP = TRUE;
      else if(strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--line-number") == 0)
         N_OP = TRUE;
      else if(strcmp(argv[i], "-q") == 0 || strcmp(argv[i], "--quiet") == 0)
         Q_OP = TRUE;
      else if (i == argc-1){//testing the last argument passed
         if (argv[i][0] == '-'){
         ERR = TRUE;
         }
         else{
            PATTERN = malloc((strlen(argv[i]) + 1) * sizeof(char));
            strcpy(PATTERN, argv[i]);
         }

      }
      else{
         ERR = TRUE;
      }
   }

   if(ERR) {
      fprintf(stderr, "Error\n");
      free(PATTERN);
      exit_status = 2;
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

   char buffer[SIZE];

   int line_num = 1; //holds the line number starting with one

   while(fgets(buffer, SIZE, stdin)){

      if (Q_OP){ //quiet mode
         if (V_OP && strstr(buffer, PATTERN) == NULL)//looking for non matching and find a nonmatching
         {
            exit_status = 0;
         }
         else if (strstr(buffer, PATTERN))//looking for matching and find matching
         {
            exit_status = 0;
         }
      }
      //nonmatching mode, found nonmatching, and line number requested
      else if (V_OP && N_OP && strstr(buffer, PATTERN) == NULL)
      {
         printf("%d: %s\n", line_num, buffer);//print w/ line number
         exit_status = 0;
      }
      //looking for nonmatching, found nonmatching, no line number requested
      else if (V_OP && strstr(buffer, PATTERN) == NULL )
      {
         printf("%s\n", buffer);//prints without line number
         exit_status = 0;
      }
      //looking for matching, found matching, line number requested
      else if (V_OP == FALSE && N_OP && strstr(buffer, PATTERN) != NULL)
      {
         printf("%d: %s\n", line_num, buffer);//print with line number
         exit_status = 0;
      }
      //looking for matching, found matching
      else if (V_OP == FALSE && strstr(buffer, PATTERN) != NULL)
      {
         printf("%s\n", buffer );
         exit_status = 0;
      }
      line_num++;
   }
   free(PATTERN);
   return exit_status;
}
