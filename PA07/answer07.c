#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdint.h>

#include "answer07.h"

#define BMP_MAGIC_NUMBER 0x4d42
#define DEFAULT_DPI_X 3780
#define DEFAULT_DPI_Y 3780

#define FALSE 0
#define TRUE 1

#pragma pack(push)
#pragma pack(1) 
typedef struct {
    uint16_t type;                      // Magic identifier
    uint32_t size;                      // File size in bytes
    uint16_t reserved1;                 // Not used
    uint16_t reserved2;                 // Not used
    uint32_t offset;                    // Offset to image data in bytes
    uint32_t header_size;               // Header size in bytes
    int32_t  width;                     // Width of the image
    int32_t  height;                    // Height of image
    uint16_t planes;                    // Number of color planes
    uint16_t bits;                      // Bits per pixel
    uint32_t compression;               // Compression type
    uint32_t imagesize;                 // Image size in bytes
    int32_t  xresolution;               // Pixels per meter
    int32_t  yresolution;               // Pixels per meter
    uint32_t ncolors;                  // Number of colors  
    uint32_t importantcolors;          // Important colors
} BMP_Header;
#pragma pack(pop)

//The Functions

static int checkValid(ImageHeader * header);


/**
 * Loads an ECE264 image file, returning an Image structure.
 * Will return NULL if there is any error.
 *
 * Hint: Please see the README for extensive hints
 */
Image * Image_load(const char * filename){

    FILE * fp = NULL;
    size_t read;
    ImageHeader header;
    Image * tmp_im = NULL, * im = NULL;
    int err = FALSE;
    char * tmp_comment;

    //opening the file
    if(!err) {
		fp = fopen(filename, "rb");
		if(!fp) { //could not open file
	    	fprintf(stderr, "Failed to open file '%s'\n", filename);
	    	err = TRUE;
		}
    }

    //reading the header
    if(!err) { // Read the header
		read = fread(&header, sizeof(ImageHeader), 1, fp);
		if(read != 1) {
	    	fprintf(stderr, "Failed to read header from '%s'\n", filename);
	    	err = TRUE;
		}
    }

    //checking the header for correct magic number and width/height values, and comment length
    if(!err) { // We're only interested in a subset of valid bmp files
		if(!checkValid(&header)) {
	    	fprintf(stderr, "Invalid header in '%s'\n", filename);
	    	err = TRUE;
		}
    }

    //Allocate space for the image, comment, and pixels
    if(!err) {
    	//allocate image struct
		tmp_im = malloc(sizeof(Image));			

		//allocate memory for comment
		tmp_comment = malloc(sizeof(char) * header.comment_len);
		
		//allocate memory for pixel data
		int * rawece264;
        rawece264 = (int*) malloc((header.height*sizeof(int)) * (header.width*sizeof(int)));
   		
   		//test memory allocation
   		if(tmp_im == NULL || tmp_comment == NULL || rawece264 == NULL) {
	    	fprintf(stderr, "Failed to allocate im structure\n");
	    	err = TRUE;
		} 

    }

    //read the comment
    if(!err){ 
		fread(tmp_comment, sizeof(char), header.comment_len, fp);
    }

    //checking the comment
    int tmp_comment_len = strlen(tmp_comment);
    if (tmp_comment[tmp_comment_len + 1] == '\0' || tmp_comment_len != header.comment_len){ //potential seg fault here
    	err = TRUE;
    }

    //read pixel data
    if(!err){
    	fread(tmp_comment, sizeof(int), (header.height * header.width), fp);
    }

    //ensure all pixel data was read and that there are no trailing bytes


    return im;
}

/**
 * Save an image to the passed filename, in ECE264 format.
 * Return TRUE if this succeeds, or FALSE if there is any error.
 *
 * Hint: Please see the README for extensive hints
 */
int Image_save(const char * filename, Image * image){

	return 0;

}

/**
 * Free memory for an image structure
 *
 * Image_load(...) (above) allocates memory for an image structure. 
 * This function must clean up any allocated resources. If image is 
 * NULL, then it does nothing. (There should be no error message.) If 
 * you do not write this function correctly, then valgrind will 
 * report an error. 
 */
void Image_free(Image * image){

	return;
}

/**
 * Performs linear normalization, see README
 */
void linearNormalization(int width, int height, uint8_t * intensity){

	return;

}

static int checkValid(ImageHeader * header){
	//Ensure that the magic_number in the header is correct
	if (header->magic_number != BMP_MAGIC_NUMBER) return FALSE;

	//Ensure that neither the width nor height is zero
	if (header->width == 0 || header->height == 0) return FALSE;

	//Ensure that there is a comment
	if(header->comment_len <= 2) return FALSE;

	return TRUE;
}



