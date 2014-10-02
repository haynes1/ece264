#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

int main(int argc, const char** argv)
{
   int i;
   int showHelp = FALSE;

   // Loop through the arguments, looking for our switches...
   for(i = 1; i < argc; i++) { //skip 0 for it is the program path

      if(argc < 1 || strcmp(argv[i], "--help") == 0){ //is it help time?
         showHelp = TRUE;
      }

      else if(strcmp(argv[i], "-") == 0){//print stdin
         char c;
         while( (c = fgetc(stdin)) != EOF){//while c isn't the EOF signal
            fputc(c, stdout); //print it
         }
      }

      else{ //print the contents of a file
         char a;
         FILE * fil;
         fil = fopen(argv[i], "r");
         //failed to open file
         if (fil == NULL)
         {
            fprintf(stderr, "cat cannot open %s\n", argv[i]);
            return EXIT_FAILURE;
         }
         while ((a = fgetc(fil)) != EOF){//while we're not at the EOF
            fputc(a, stdout);// put a in the command line
         }
         fclose(fil);//close
      }

   }

   if(showHelp == TRUE){
      printf("Usage: cat-lite [--help] [FILE]...\n"
             "With no FILE, or when FILE is -, read standard input.\n");
      return EXIT_SUCCESS;
   }
   printf("\n");

   return EXIT_SUCCESS;
}


