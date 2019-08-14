#ifndef ALIGNED
#define ALIGNED __attribute__ ((aligned(16)))
#endif

#include "ppu/pnm.h"
#include "ppu/ccc.h"


typedef struct ALIGNED {
	pnm_img_t *in_pnm_addr;
	ccc_img_t *out_ccc_addr;
	unsigned int nr_blocks;
	unsigned int nr_spus;
	unsigned int id;
	unsigned char mod_vect;
	unsigned char mod_dma;
} io_addr_t;


