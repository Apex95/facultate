#ifndef _PNM_H
#define _PNM_H

#ifndef ALIGNED
#define ALIGNED __attribute__ ((aligned(16)))
#endif


// struct that holds a PNM image
typedef struct ALIGNED pnm_img {
	unsigned int width;
	unsigned int height;

	unsigned char *red;
	unsigned char *green;
	unsigned char *blue;
} pnm_img_t;

// function headers for pnm.c
void read_pnm(char* path, pnm_img_t* in_img);
void write_pnm(char* path, pnm_img_t* out_img);
void free_pnm(pnm_img_t* out_img);

#endif
