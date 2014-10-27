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
    size_t n_bytes = 0;
    ImageHeader header;
    Image * tmp_im = NULL, * im = NULL;
    int err = FALSE;

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
	    	err = 2;
		}
    }

    //checking the header for correct magic number and width/height values, and comment length
    if(!err) {
		if(!checkValid(&header)) {
	    	fprintf(stderr, "Invalid header in '%s'\n", filename);
	    	err = 3;
		}
    }

    //Allocate space for the image, comment, and pixels
    if(!err) {
    	//allocate image struct
		tmp_im = malloc(sizeof(Image));			

		//allocate memory for comment
		tmp_im->comment = malloc(sizeof(char) * (header.comment_len));//segfault potential here
		
		//allocate memory for pixel data
		n_bytes = sizeof(uint8_t) * header.width * header.height;
		tmp_im->data = malloc(n_bytes);
		if(tmp_im->data == NULL) {
	    	fprintf(stderr, "Failed to allocate %zd bytes for image data\n", n_bytes);
	    	err = 4;
		}
   		
   		//test memory allocation
   		if(tmp_im == NULL || tmp_im->comment == NULL || tmp_im->data == NULL) {
	    	fprintf(stderr, "Failed to allocate im structure\n");
	    	err = 5;
		} 

    }

    if(!err) { // Init the Image struct
	tmp_im->width = header.width;
	tmp_im->height = header.height;
	}

    //read the comment
    if(!err){ 
		fread(tmp_im->comment, sizeof(char), header.comment_len, fp);
    }

    //checking the comment
    if(!err){
    	int tmp_comment_len = strlen(tmp_im->comment);
    	char comment_end = tmp_im->comment[tmp_comment_len-1];
    	if (comment_end == '\0'){ //potential seg fault here
    		err = 6;
    	}
    	if( (tmp_comment_len + 1) != header.comment_len){
    		err = 7;
    	}
	}

    //read pixel data
    if(!err){
    	fread(tmp_im->data, sizeof(int), (header.height * header.width), fp);
    }

    //ensure all pixel data was read and that there are no trailing bytes
    if(!err) { // We should be at the end of the file now
		uint8_t byte;
		read = fread(&byte, sizeof(uint8_t), 1, fp);
		if(read != 0) {
	    	fprintf(stderr, "Stray bytes at the end of bmp file '%s'\n", filename);
	    	err = 8;
		}
    }

    //put it all into im
    if(!err) {
		im = tmp_im;  
		tmp_im = NULL; // and not cleaned up
    }

    // Cleanup
    if(tmp_im != NULL) {
		free(tmp_im->comment); // Remember, you can always free(NULL)
		free(tmp_im->data);
		free(tmp_im);
    }

    if(fp) {
		fclose(fp);
    }
    return im;
}

/**
 * Save an image to the passed filename, in ECE264 format.
 * Return TRUE if this succeeds, or FALSE if there is any error.
 *
 * Hint: Please see the README for extensive hints
 */
int Image_save(const char * filename, Image * image){
	int err = FALSE; //unless otherwise stated
	FILE * fp = NULL;
	size_t written = 0;

	//opening the file for writing
	fp = fopen(filename, "wb");
    if(fp == NULL) {
		fprintf(stderr, "Failed to open '%s' for writing\n", filename);
		return FALSE; // No file ==> out of here.
    }

    //making the header
    ImageHeader imagehead;
    imagehead.magic_number = ECE264_IMAGE_MAGIC_NUMBER;
    imagehead.width = image->width;
    imagehead.height = image->height;
    int commentlen = strlen(image->comment);
    imagehead.comment_len = commentlen;

    //writing the header
    if(!err) {
    	//writing head
		written = fwrite(&imagehead, sizeof(ImageHeader), 1, fp);
		if(written != 1) {
	    	fprintf(stderr, "Error: only wrote %zd of %zd of file header to '%s'\n", written, sizeof(ImageHeader), filename);
	    	err = TRUE;	
		}
		//writing comment
		written = fwrite(image->comment, sizeof(char), commentlen + 1, fp);
    }

    //write pixels
    if(!err){
    	uint8_t * data_ptr = image->data;
    	written = fwrite(data_ptr, sizeof(uint8_t), (imagehead.width * imagehead.height), fp);
    	if(written != (imagehead.width * imagehead.height)) {
	    	fprintf(stderr, "Error: only wrote %zd of %zd of file header to '%s'\n", written, sizeof(BMP_Header), filename);
	    	err = TRUE;	
		}
    }

    return !err;
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

    free(image->comment);
    free(image->data);
    free(image);

	return;
}

/**
 * Performs linear normalization, see README
 */
void linearNormalization(int width, int height, uint8_t * intensity){
	int min = 300;
	int max = 0;
	int i;
	//finding min value
	for (i = 0; i <= (width * height); i++)
	{
		if (intensity[i] < min)
		{
			min = intensity[i];
		}
	}
	//finding max value
	for (i = 0; i <= (width * height); i++)
	{
		if (intensity[i] > max)
		{
			max = intensity[i];
		}
	}
	//normalize
	for (i = 0; i <= (width * height); i++)
	{
		intensity[i] = (intensity[i] - min) * 255.0 / (max - min);
	}


	//printf("min = %d, max = %d\n", min , max);

	return;
}

static int checkValid(ImageHeader * header){
	//Ensure that the magic_number in the header is correct
	if (header->magic_number != ECE264_IMAGE_MAGIC_NUMBER) return FALSE;

	//Ensure that neither the width nor height is zero
	if (header->width == 0 || header->height == 0) return FALSE;

	//Ensure that there is a comment
	if(header->comment_len <= 1) return FALSE;

	return TRUE;
}



