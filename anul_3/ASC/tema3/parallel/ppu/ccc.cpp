#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "ccc.h"
#include "utils.h"

void write_ccc(char* path, ccc_img_t* out_img){
	int i, nr_blocks, j, fd, k;
	struct bits tmp;
	char *buf;
	char dim_buf[CCC_BUF_SIZE];
		
	fd = _open_for_write(path);

	// write image width
	snprintf(dim_buf, CCC_BUF_SIZE, "%d\n", out_img->width);
	write(fd, dim_buf, strlen(dim_buf));

	// write image height
	snprintf(dim_buf, CCC_BUF_SIZE, "%d\n", out_img->height);
	write(fd, dim_buf, strlen(dim_buf));

	// calculate number of blocks
	nr_blocks = out_img->width * out_img->height / (BLOCK_SIZE * BLOCK_SIZE);
	buf = (char*) _alloc(nr_blocks * BLOCK_SIZE_IN_FILE);

	k = 0;
	for (i=0; i<nr_blocks; i++){
		//write (r1, g1, b1) and (r2, g2, b2)
		buf[k++] = out_img->blocks[i].r1;
		buf[k++] = out_img->blocks[i].g1;
		buf[k++] = out_img->blocks[i].b1;
		buf[k++] = out_img->blocks[i].r2;
		buf[k++] = out_img->blocks[i].g2;
		buf[k++] = out_img->blocks[i].b2;
		
		//from bytes to bits
		j = 0;
		while (j < BLOCK_SIZE * BLOCK_SIZE){
			tmp.bit0 = out_img->blocks[i].bitmap[j++];
			tmp.bit1 = out_img->blocks[i].bitmap[j++];
			tmp.bit2 = out_img->blocks[i].bitmap[j++];
			tmp.bit3 = out_img->blocks[i].bitmap[j++];
			tmp.bit4 = out_img->blocks[i].bitmap[j++];
			tmp.bit5 = out_img->blocks[i].bitmap[j++];
			tmp.bit6 = out_img->blocks[i].bitmap[j++];
			tmp.bit7 = out_img->blocks[i].bitmap[j++];						
			buf[k++] = *((char*)&tmp);
		}
	}

	// write entire buffer for faster writing
	_write_buffer(fd, buf, nr_blocks * BLOCK_SIZE_IN_FILE);

	free(buf);
	close(fd);
}

void read_ccc(char* path, ccc_img_t* out_img){
	int fd, nr_blocks, i, j = 0, k, ii;
	char *big_buf, dim_buf[CCC_BUF_SIZE];
	struct bits tmp;
	
	fd = _open_for_read(path);
	
	// read width and height
	_read_line(fd, path, dim_buf, CCC_BUF_SIZE);
	sscanf(dim_buf, "%d\n", &out_img->width);
	_read_line(fd, path, dim_buf, CCC_BUF_SIZE);
	sscanf(dim_buf, "%d\n", &out_img->height);

	// calculate number of blocks, allocate block data, read buffer
	nr_blocks = out_img->width * out_img->height / (BLOCK_SIZE * BLOCK_SIZE);
	out_img->blocks = (ccc_block_t*) _alloc(nr_blocks * sizeof(ccc_block_t));
	big_buf = (char*) _alloc(nr_blocks * BLOCK_SIZE_IN_FILE);
	
	// read entire file contents in a buffer for faster reading
	_read_buffer(fd, big_buf, nr_blocks * BLOCK_SIZE_IN_FILE);

	// extract block data from buffer
	for (i=0; i<nr_blocks; i++){
		//read a and b
		out_img->blocks[i].r1 = big_buf[j++];
		out_img->blocks[i].g1 = big_buf[j++];
		out_img->blocks[i].b1 = big_buf[j++];
		out_img->blocks[i].r2 = big_buf[j++];
		out_img->blocks[i].g2 = big_buf[j++];
		out_img->blocks[i].b2 = big_buf[j++];

		//read bitmap
		k = 0;
		for (ii=0; ii<BLOCK_SIZE * BLOCK_SIZE / BITS_IN_BYTE; ii++){
			tmp = *((struct bits*)&big_buf[j++]);
			out_img->blocks[i].bitmap[k++] = tmp.bit0;
			out_img->blocks[i].bitmap[k++] = tmp.bit1;
			out_img->blocks[i].bitmap[k++] = tmp.bit2;
			out_img->blocks[i].bitmap[k++] = tmp.bit3;
			out_img->blocks[i].bitmap[k++] = tmp.bit4;
			out_img->blocks[i].bitmap[k++] = tmp.bit5;
			out_img->blocks[i].bitmap[k++] = tmp.bit6;
			out_img->blocks[i].bitmap[k++] = tmp.bit7;			
		}
	}
	
	// free temporary bufffer
	free(big_buf);
	
	close(fd);
}

void free_ccc(ccc_img_t* image){
	free(image->blocks);
}
