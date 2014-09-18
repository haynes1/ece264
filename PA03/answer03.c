#include "answer03.h"
#include <stdlib.h>
#include <string.h>

/**
 * Append the C-string 'src' to the end of the C-string '*dest'.
 *
 * strcat_ex(...) will append the C-string 'src' to the end of the string
 * at '*dest'. The parameter 'n' is the address of a int that specifies how
 * many characters can safely be stored in '*dest'. 
 *
 * If '*dest' is NULL, or if '*dest' is not large enough to contain the result
 * (that is, the sum of the lengths of *dest, src, and the null byte), then
 * strcat_ex will:
 * (1) malloc a new buffer of size 1 + 2 * (strlen(*dest) + strlen(src))
 * (2) set '*n' to the size of the new buffer
 * (3) copy '*dest' into the beginning of the new buffer
 * (4) free the memory '*dest', and then set '*dest' to point to the new buffer
 * (5) concatenate 'src' onto the end of '*dest'.
 *
 * Always returns *dest.
 *
 * Why do we need to pass dest as char * *, and n as int *? 
 * Please see the FAQ for an answer.
 *
 * Hint: These <string.h> functions will help: strcat, strcpy, strlen.
 * Hint: Leak no memory.
 */
char * strcat_ex(char * * dest, int * n, const char * src)
{
	//the length of the concatenated string including the single /0 at the end
	int destlen = strlen(* dest) + 1;
	int total_len = strlen(*dest) + strlen(src) + 1;

	if (* dest == NULL || destlen < total_len)//if we need to allocate more memory
	{
		char * newdest = malloc(1 + 2 * (strlen(*dest) + strlen(src)));
		strcpy(newdest, * dest);
		* dest = newdest;
		strcat(*dest, src); //concatenate src onto new dest


		return * dest;
	}
	else
	{
		strcat(* dest, src);
		return * dest;
	}
	* n = strlen(* dest);
}

/**
 * Takes a string and splits it into an array of strings according to delimiter.
 * The memory location '*arrLen' is initialized to the length of the returned
 * array.
 *
 * str: A string to split
 * delims: a string that contains a set of delimiter characters. explode(...) 
 *         will split the string at any character that appears in 'delims'.
 * arrLen: pointer to an int that is used to store the resultant length of the
 *         returned array.
 *
 * For example, if delimiter is white space " \t\v\n\r\f", then,
 * int len;
 * char * * strArr = explode("The\nTuring test", " \t\v\n\r\f", &len);
 * // len = 3, strArr[0] is "The", strArr[1] is "Turing", strArr[2] is "test"
 *
 * Hint: you can use <string.h> functions "memcpy" and "strchr"
 *       "memcpy" copies blocks of memory.
 *       "strchr" can be used to tell if a specific character is in delims.
 * Hint: this question is hard; it will help to draw out your algorithm.
 * Hint: read the FAQ...
 */
char * * explode(const char * str, const char * delims, int * arrLen)
{
	int delim_indicies[strlen(str)]; //an array that holds all indicies of all delimiters
	int num_delims = 0; //the number of delimiters
	int i;
	int j;
	int k;

	//iterate through str and test each entry to see if it is a delimiter
	for (i = 0; i < strlen(str); i++)
	{
		//iterate through delim while on each index of str
		for (j = 0; j < strlen(delims); j++)
		{
			if (str[i] == delims[j])//if it is a delimiter
			{
				delim_indicies[num_delims] = i; //that index is added to delim_indicies
				num_delims++;
			}
		}
	}


	char * * exploded = malloc((num_delims+1) * sizeof(char *)); //2D string array
	int first = 0; //the beginning of a substring
	int last = 0; //the end of a substring +1
	int subsize; //the size of the substring

	for (k = 0; k <= num_delims; k++)//iterates once more thant the number of delimiters
	{
		if (k != num_delims)// if this is not the last substring
		{
			last = delim_indicies[k];//set last to the end of the first substring and include the delimiter
		}
		else
		{
			last = strlen(str);
		}

		subsize = last - first; //use first and last to find the size of the substring

		exploded[k] = (char *) malloc(subsize * sizeof(char));
		memcpy(exploded[k], &str[first], subsize);
		exploded[last+1] = '\0';

		first = last + 1; //moves first to the index after the delimiter
	}

	//still must do the arrLen thing
	* arrLen = num_delims + 1;


	return exploded;
}

/**
 * Takes an array of strings, and concatenates the elements into a single
 * string, placing 'glue' between each successive element.
 *
 * strArr: an array of strings
 * len: the length of the array 'strArr'
 * glue: string to concatenate between each element of 'strArr'
 *
 * For example:
 * int len;
 * char * * strArr = explode("100 224 147 80", " ", &len);
 * char * str = implode(strArr, len, ", ");
 * printf("(%s)\n", str); // (100, 224, 147, 80)
 *
 * Hint: use strcat_ex in a for loop.
 */
char * implode(char * * strArr, int len, const char * glue)
{
	return 0;
}

/**
 * Takes an array of C-strings, and sorts them alphabetically, ascending.
 *
 * arrString: an array of strings
 * len: length of the array 'arrString'
 *
 * For example, 
 * int len;
 * char * * strArr = explode("lady beatle brew", " ", &len);
 * sortStringArray(strArr, len);
 * char * str = implode(strArr, len, " ");
 * printf("%s\n"); // beatle brew lady
 *
 * Hint: use the <stdlib.h> function "qsort"
 * Hint: you must _clearly_ understand the typecasts.
 */
void sortStringArray(char * * arrString, int len)
{
	return;
}

/**
 * Sorts the characters in a string.
 *
 * str: string whose characters are to be sorted
 *
 * For example, 
 * char * s1 = strdup("How did it get so late so soon?");
 * sortStringCharacters(s1)
 * // s1 is now "       ?Haddeegiilnooooossstttw"
 *
 * Hint: use the <stdlib.h> function "qsort"
 * Hint: you must _clearly_  understand the typecasts.
 */
void sortStringCharacters(char * str)
{
	return;
}

/**
 * Safely frees all memory associated with strArr, and then strArr itself.
 * Passing NULL as the first parameter has no effect.
 *
 * strArr: an array of strings
 * len: the length of 'strArr'
 *
 * int len;
 * const char * abe = "Give me six hours to chop down a tree and I will spend\n"
 *                    "the first four sharpening the axe.";
 * char * * strArr = explode(abe, "\n ");
 * destroyStringArray(strArr, len); // cleans memory -- no memory leaks
 * destroyStringArray(NULL, 0); // does nothing, does not crash.
 */
void destroyStringArray(char * * strArr, int len)
{
	return;
}
