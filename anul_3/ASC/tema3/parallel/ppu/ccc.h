#ifndef _CCC_H
#define _CCC_H

#ifndef ALIGNED
#define ALIGNED __attribute__ ((aligned(16)))
#endif



#define CCC_BUF_SIZE 	256
#define BLOCK_SIZE 	16
#define BITS_IN_BYTE	8
/* calculate number of blocks and allocate buffer used for writing blocks   
 * 1 block has 6 bytes for r1, g1, b1, r2, g2, b2 and 4 * 4 / 8 = 2 bytes for the bitmap
 * a total of 8 bytes
 */
#define BLOCK_SIZE_IN_FILE (6 * sizeof(char) + (BLOCK_SIZE * BLOCK_SIZE) / BITS_IN_BYTE)

// block in a compressed image
typedef struct ALIGNED ccc_block{
	// color 1
	unsigned char r1, g1, b1;
	// color 2
	unsigned char r2, g2, b2;
	// bitmap - one byte to each bit
	unsigned char bitmap[BLOCK_SIZE * BLOCK_SIZE];
} ccc_block_t;

// compressed image
typedef struct ALIGNED ccc_img {
	unsigned int width, height;
	ccc_block_t* blocks;
} ccc_img_t;

struct bits{
	unsigned bit0 : 1;
	unsigned bit1 : 1;
	unsigned bit2 : 1;
	unsigned bit3 : 1;
	unsigned bit4 : 1;
	unsigned bit5 : 1;
	unsigned bit6 : 1;
	unsigned bit7 : 1;
};

//functions from ccc.c
void read_ccc(char* path, ccc_img_t* out_img);
void write_ccc(char* path, ccc_img_t* out_img);
void free_ccc(ccc_img_t* out_img);

#endif

