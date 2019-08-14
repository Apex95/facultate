#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"
#include "pnm.h"

#define PNM_BUF_SIZE 256
 
void read_pnm(char* path, pnm_img_t* in_img){
	int fd;
	unsigned int i, j;
	char buf[PNM_BUF_SIZE], *token;
	unsigned char *pixels;
	
	fd = _open_for_read(path);

	// read file type; expecting P6
	_read_line(fd, path, buf, PNM_BUF_SIZE);
	if (strncmp(buf, "P6", 2)){
		fprintf(stderr, "Expected binary PNM (P6 type), got %s\n", path);
		exit(0);
	}

	// read comment line
	_read_line(fd, path, buf, PNM_BUF_SIZE);

	// read image width and height
	_read_line(fd, path, buf, PNM_BUF_SIZE);
	token = strtok(buf, " ");
	if (token == NULL){
		fprintf(stderr, "Expected token when reading from %s\n", path);
		exit(0);
	}	
	in_img->width = atoi(token);
	token = strtok(NULL, " ");
	if (token == NULL){
		fprintf(stderr, "Expected token when reading from %s\n", path);
		exit(0);
	}
	in_img->height = atoi(token);
	if (in_img->width <= 0 || in_img->height <= 0){
		fprintf(stderr, "Invalid width or height when reading from %s\n", path);
		exit(0);
	}

	// read max value
	_read_line(fd, path, buf, PNM_BUF_SIZE);

	// allocate memory for temporary pixels array
	pixels = (unsigned char*) _alloc(in_img->width * in_img->height *
			3 * sizeof (char));

	// allocate memory for red, green and blue channels
	in_img->red = (unsigned char*) _alloc_align(in_img->width * in_img->height *
		sizeof (char));
	in_img->green = (unsigned char*) _alloc_align(in_img->width * in_img->height *
		sizeof (char)); 
	in_img->blue = (unsigned char*) _alloc_align(in_img->width * in_img->height *
			sizeof (char)); 

	// read the RGB data in a continuous area for faster reading
	_read_buffer(fd, pixels, in_img->width * in_img->height * 3);

	// store each channel separately
	for (i = 0, j = 0; i < in_img->width * in_img->height * 3; i += 3, j++) {
		in_img->red[j] = pixels[i];
		in_img->green[j] = pixels[i + 1];
		in_img->blue[j] = pixels[i + 2];
	}
	
	// free temporary pixels array
	free(pixels);

	close(fd);
}

void write_pnm(char* path, pnm_img_t* out_img){
	int fd;
	unsigned int i, j; 
	char buf[PNM_BUF_SIZE];
	unsigned char *pixels;

	fd = _open_for_write(path);

	// write image type
	strcpy(buf, "P6\n");
	_write_buffer(fd, buf, strlen(buf));

	// write comment 
	strcpy(buf, "#Created using ASC Tema3\n");
	_write_buffer(fd, buf, strlen(buf));

	// write image width and height
	sprintf(buf, "%d %d\n", out_img->width, out_img->height);
	_write_buffer(fd, buf, strlen(buf));

	// write max value
	strcpy(buf, "255\n");
	_write_buffer(fd, buf, strlen(buf));

	// allocate memory for temporary pixels array
	pixels = (unsigned char*) _alloc(out_img->width * out_img->height *
			3 * sizeof (char));

	// store all RGB data in a continuous area for faster writing
	for (i = 0, j = 0; i < out_img->width * out_img->height * 3; i += 3, j++) {
		pixels[i] = out_img->red[j];
		pixels[i + 1] = out_img->green[j];
		pixels[i + 2] = out_img->blue[j];
	}

	// write image pixels
	_write_buffer(fd, pixels, out_img->width * out_img->height * 3);

	// free temporary pixels array
	free(pixels);

	close(fd);
}

void free_pnm(pnm_img_t* image){
	free(image->red);
	free(image->green);
	free(image->blue);
}

