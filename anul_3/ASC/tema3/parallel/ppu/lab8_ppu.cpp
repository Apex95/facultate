#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <libspe2.h>
#include <pthread.h>
#include <libmisc.h>
#include <string.h>
#include "utils.h"
#include "pnm.h"
#include "ccc.h"
#include "../lab8_common.h"
#include <sys/time.h>


extern spe_program_handle_t lab8_spu;

spe_context_ptr_t * contexts;
unsigned int nr_spus_finished;
pthread_mutex_t nr_spus_finished_lock;
pthread_mutex_t work_in_progress_lock;
unsigned int num_spus;
char _threads_spawned;

void color_cell_decompress(pnm_img_t* pnm_image, ccc_img_t* ccc_image){
	unsigned int block_row_start, block_col_start, i, j, nr_blocks, k;
	ccc_block_t* curr_block;

	// calculate number of blocks
	nr_blocks = ccc_image->width * ccc_image->height / (BLOCK_SIZE * BLOCK_SIZE);

	// set width and height
	pnm_image->width = ccc_image->width;
	pnm_image->height = ccc_image->height;

	// allocate RGB
	pnm_image->red = (unsigned char*)_alloc(pnm_image->width * pnm_image->height * sizeof(char));
	pnm_image->green = (unsigned char*)_alloc(pnm_image->width * pnm_image->height * sizeof(char));
	pnm_image->blue = (unsigned char*)_alloc(pnm_image->width * pnm_image->height * sizeof(char));

	block_row_start = block_col_start = 0;
	for (i=0; i<nr_blocks; i++){
		k = block_row_start * ccc_image->width + block_col_start;
		curr_block = &ccc_image->blocks[i];

		for (j=0; j<BLOCK_SIZE * BLOCK_SIZE; j++){
			pnm_image->red[k] = (curr_block->bitmap[j] ? curr_block->r1 : curr_block->r2);
			pnm_image->green[k] = (curr_block->bitmap[j] ? curr_block->g1 : curr_block->g2);
			pnm_image->blue[k] = (curr_block->bitmap[j] ? curr_block->b1 : curr_block->b2);
			k++;
			if ((j + 1) % BLOCK_SIZE == 0){
				k -= BLOCK_SIZE; //back to the first column of the block
				k += ccc_image->width ; //go to the next line
			}
		}
		block_col_start += BLOCK_SIZE;
		if (block_col_start >= ccc_image->width){
			block_col_start = 0;
			block_row_start += BLOCK_SIZE;
		}
	}

}

void *ppu_pthread_comm(void *id) {

	int thread_id = (int)id;


	unsigned int status = 0;

	

	while(1) {
		status = 0;
		spe_out_mbox_read(contexts[thread_id], &status, 1);

		if (status == 1)
		{

			pthread_mutex_lock(&nr_spus_finished_lock);
			nr_spus_finished++;

			if (nr_spus_finished == num_spus)
			{
				pthread_mutex_unlock(&work_in_progress_lock);
				nr_spus_finished = 0;
			}

			pthread_mutex_unlock(&nr_spus_finished_lock);
		}

		if (status == 2)
		{

			status = 2;
			spe_in_mbox_write(contexts[thread_id], &status, 1, SPE_MBOX_ALL_BLOCKING);

			break;
		}
		
	}

	pthread_exit(NULL);
}

void *ppu_pthread_function(void *thread_arg) {

	io_addr_t *arg = (io_addr_t *) thread_arg;


	/* Load SPE program into context */
	if (spe_program_load(contexts[arg->id], &lab8_spu)) {
		perror("Failed loading program");
		exit(1);
	}

	/* Run SPE context */
	unsigned int entry = SPE_DEFAULT_ENTRY;
	

	if (spe_context_run(contexts[arg->id], &entry, 0, (void*)arg, NULL, NULL) < 0) {  
		perror("Failed running context");
		exit(1);
	}


	/* Destroy context */
	if (spe_context_destroy (contexts[arg->id]) != 0) {
		perror("Failed destroying context");
		exit (1);
	}


	pthread_exit(NULL);
}


int main(int argc, char * argv[])
{
	int mod_vect, mod_dma;
	char img_name[300];

	sscanf(argv[1], "%d", &num_spus);
	sscanf(argv[2], "%d", &mod_vect);
	sscanf(argv[3], "%d", &mod_dma);

	_threads_spawned = 0;
	pthread_mutex_init(&nr_spus_finished_lock, NULL);
	pthread_mutex_init(&work_in_progress_lock, NULL);

	// structura adrese pt cele 2 poze (pnm + ccc)
	io_addr_t io_addrs[num_spus] ALIGNED;

	struct timeval t1, t2;


	int img_index;
	unsigned int i, spu_threads = num_spus;
	pthread_t threads[num_spus];
	pthread_t comm_threads[num_spus];

	contexts = (spe_context_ptr_t *)malloc(sizeof(spe_context_ptr_t) * num_spus);

	for (img_index = 4; img_index < argc; img_index++) {
		// -- procesare poza
		pthread_mutex_lock(&work_in_progress_lock);

		strcpy(img_name, argv[img_index]);

		pnm_img_t in_pnm_img;
		printf("--- File: %s ---\n", img_name);
	
		// citire poza
		read_pnm(img_name, &in_pnm_img);

		gettimeofday(&t1, NULL);

		ccc_img_t out_ccc_img;
		out_ccc_img.width = in_pnm_img.width;
		out_ccc_img.height = in_pnm_img.height;



		unsigned int nr_blocks = in_pnm_img.width * in_pnm_img.height / (BLOCK_SIZE * BLOCK_SIZE);

		out_ccc_img.blocks = (ccc_block_t*) _alloc_align(nr_blocks * sizeof(ccc_block_t));

		for(i = 0; i < spu_threads; i++) {

			io_addrs[i].in_pnm_addr = &in_pnm_img;
			io_addrs[i].out_ccc_addr = &out_ccc_img;
			io_addrs[i].id = i;
			io_addrs[i].nr_blocks = nr_blocks;
			io_addrs[i].nr_spus = num_spus;
			io_addrs[i].mod_vect = mod_vect;
			io_addrs[i].mod_dma = mod_dma;


			if(!_threads_spawned) {

				if ((contexts[i] = spe_context_create(0, NULL)) == NULL) {
					perror("Failed creating context");
					exit(1);
				}

				// threaduri pt SPU                
				if (pthread_create(&threads[i], NULL, &ppu_pthread_function, &(io_addrs[i])))  {
					perror("Failed creating thread");
					exit(1);
				}

				// threaduri pt comunicarea cu SPU 
				if (pthread_create(&comm_threads[i], NULL, &ppu_pthread_comm, (void*)i))  {
					perror("Failed creating thread");
					exit(1);
				}

			}

			// informez spu ca poate sa citeasca de pe magistrala si sa proceseze
			unsigned int status = 1;
			spe_in_mbox_write(contexts[i], &status, 1, SPE_MBOX_ALL_BLOCKING);
			
		}

		if (!_threads_spawned)
			_threads_spawned = 1;

	
		pthread_mutex_lock(&work_in_progress_lock);
		pthread_mutex_unlock(&work_in_progress_lock);

		gettimeofday(&t2, NULL);

		printf("Compress time: %lf\n", GET_TIME_DELTA(t1, t2));

		// -- scriere ccc
		sprintf(img_name, "out%d.ccc", img_index - 3);
		write_ccc(img_name, &out_ccc_img);

		// -- decomprimare
		sprintf(img_name, "out%d.pnm", img_index - 3);
		pnm_img_t out_pnm_img;

		color_cell_decompress(&out_pnm_img, &out_ccc_img);
		write_pnm(img_name, &out_pnm_img);

		free_align(out_ccc_img.blocks);
	
	}


	unsigned int status = 0;
	for (i = 0; i < spu_threads; i++)
		spe_in_mbox_write(contexts[i], &status, 1, SPE_MBOX_ALL_BLOCKING);

	printf("-- no more imgs --\n");


	/* Wait for SPU-thread to complete execution.  */
	for (i = 0; i < spu_threads; i++)
		if (pthread_join (threads[i], NULL)) {
			perror("Failed pthread_join");
			exit (1);
		}

	for (i = 0; i < spu_threads; i++)
		if (pthread_join (comm_threads[i], NULL)) {
			perror("Failed pthread_join");
			exit (1);
		}

	free(contexts);

	return 0;
}
