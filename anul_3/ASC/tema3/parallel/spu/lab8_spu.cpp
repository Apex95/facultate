#include <stdio.h>
#include <spu_intrinsics.h>
#include <spu_mfcio.h>
#include "../lab8_common.h"

#define wait_tag(t) mfc_write_tag_mask(1<<t); mfc_read_tag_status_all();



int main(unsigned int speid, unsigned long long argp, unsigned long long envp)
{
	uint32_t tag[2];
	tag[0] = mfc_tag_reserve();
	tag[1] = mfc_tag_reserve();
	
	if (tag[0] == MFC_TAG_INVALID || tag[1] == MFC_TAG_INVALID){
		printf("SPU: ERROR can't allocate tag ID\n"); 
		return -1;
	}

	unsigned char red[2][BLOCK_SIZE * BLOCK_SIZE] ALIGNED;
	unsigned char green[2][BLOCK_SIZE * BLOCK_SIZE] ALIGNED;
	unsigned char blue[2][BLOCK_SIZE * BLOCK_SIZE] ALIGNED;

	

	// obtin adresa pt io_addrs (pt locatia structurilor pnm si ccc)
	io_addr_t io_addrs;

	unsigned int status;


	while(1) {
		status = spu_read_in_mbox();
		
		// nu mai exista alte fisiere	
		if (status == 0)
			break;


		mfc_get((void *) &io_addrs, (unsigned int)argp, sizeof(io_addr_t), tag[0], 0, 0);  
		wait_tag(tag[0]);

		// setez id-ul ca fiind cel atribuit in ppu	
		speid = io_addrs.id;


		// obtin adresa pt pnm
		pnm_img_t in_pnm;
		mfc_get((void *) &in_pnm, (unsigned int)(io_addrs.in_pnm_addr), sizeof(in_pnm), tag[0], 0, 0);  

		// obtin adresa pt ccc
		ccc_img_t out_ccc;
		mfc_get((void *) &out_ccc, (unsigned int)(io_addrs.out_ccc_addr), sizeof(out_ccc), tag[0], 0, 0);  	

		wait_tag(tag[0]);

		

		unsigned int block_index = speid;
		ccc_block_t curr_block;


		// -- primul block pt double buffering --
		int i = 0, buf = 0, nxt_buf;
		unsigned int block_x = block_index * BLOCK_SIZE % in_pnm.width;
		unsigned int block_y = block_index * BLOCK_SIZE / in_pnm.width * BLOCK_SIZE;
		
		if (io_addrs.mod_dma == 1)
		{
			for (i = 0; i < BLOCK_SIZE; i++) {
		
				mfc_get((void *)(red[buf] + i * BLOCK_SIZE), (unsigned int)(in_pnm.red + (i + block_y) * in_pnm.width + block_x), BLOCK_SIZE * sizeof(unsigned char), tag[buf], 0, 0);

				mfc_get((void *)(green[buf] + i * BLOCK_SIZE), (unsigned int)(in_pnm.green + (i + block_y) * in_pnm.width + block_x), BLOCK_SIZE * sizeof(unsigned char), tag[buf], 0, 0);

				mfc_get((void *)(blue[buf] + i * BLOCK_SIZE), (unsigned int)(in_pnm.blue + (i + block_y) * in_pnm.width + block_x), BLOCK_SIZE * sizeof(unsigned char), tag[buf], 0, 0);
			}

			block_index += io_addrs.nr_spus;
		}


		// citirea blockurilor +  o iteratie in plus daca e setat double buffering
		// (astfel incat sa pot procesa si ultimul block)
		while (block_index < io_addrs.nr_blocks + io_addrs.nr_spus * io_addrs.mod_dma)
		{
			if (io_addrs.mod_dma == 1)
				nxt_buf = buf ^ 1;
			else
				nxt_buf = buf;

			block_x = block_index * BLOCK_SIZE % in_pnm.width;
			block_y = block_index * BLOCK_SIZE / in_pnm.width * BLOCK_SIZE;


			// citire linii din block
			for (i = 0; i < BLOCK_SIZE && block_index < io_addrs.nr_blocks; i++) {
		
				mfc_get((void *)(red[nxt_buf] + i * BLOCK_SIZE), (unsigned int)(in_pnm.red + (i + block_y) * in_pnm.width + block_x), BLOCK_SIZE * sizeof(unsigned char), tag[nxt_buf], 0, 0);
				//wait_tag(tag[nxt_buf]);

				mfc_get((void *)(green[nxt_buf] + i * BLOCK_SIZE), (unsigned int)(in_pnm.green + (i + block_y) * in_pnm.width + block_x), BLOCK_SIZE * sizeof(unsigned char), tag[nxt_buf], 0, 0);
				//wait_tag(tag[nxt_buf]);

				mfc_get((void *)(blue[nxt_buf] + i * BLOCK_SIZE), (unsigned int)(in_pnm.blue + (i + block_y) * in_pnm.width + block_x), BLOCK_SIZE * sizeof(unsigned char), tag[nxt_buf], 0, 0);
				//wait_tag(tag[nxt_buf]);
			}
			
			

			int row = 0, col = 0;

			// -- calcul luminante + medie --
			float luminances[BLOCK_SIZE*BLOCK_SIZE] ALIGNED;
			float avg_luminance = 0;

			int nr_ones = 0;

			// astept sa ajunga blockul ce trebuie procesat	
			wait_tag(tag[buf]);

			if (io_addrs.mod_vect == 0)
			{
				// mod scalar
				for (row = 0; row < BLOCK_SIZE; row++)
					for (col = 0; col < BLOCK_SIZE; col++)
						luminances[row * BLOCK_SIZE + col] = 
							((float)red[buf][row * BLOCK_SIZE + col]) * 0.30 + 
							((float)green[buf][row * BLOCK_SIZE + col]) * 0.59 + 
							((float)blue[buf][row * BLOCK_SIZE + col]) * 0.11;


				for (i = 0; i < BLOCK_SIZE * BLOCK_SIZE; i++)
					avg_luminance += luminances[i];	

				avg_luminance /= (float)(BLOCK_SIZE * BLOCK_SIZE);


				for (i = 0; i < BLOCK_SIZE * BLOCK_SIZE; i++) {
					curr_block.bitmap[i] =	((luminances[i] >= avg_luminance) ? 1 : 0);
					nr_ones += curr_block.bitmap[i];
				}
			}
			else {
				// mod vectorial
				vector float *v_luminances = (vector float *) luminances;

				float _coef_red[4] ALIGNED = {0.30, 0.30, 0.30, 0.30} ;
				float _coef_green[4] ALIGNED = {0.59, 0.59, 0.59, 0.59};
				float _coef_blue[4] ALIGNED = {0.11, 0.11, 0.11, 0.11};

				vector float *v_coef_red = (vector float *) _coef_red;
				vector float *v_coef_green = (vector float *) _coef_green;
				vector float *v_coef_blue = (vector float *) _coef_blue;

			
				unsigned int ui_red[BLOCK_SIZE * BLOCK_SIZE];
				unsigned int ui_green[BLOCK_SIZE * BLOCK_SIZE];
				unsigned int ui_blue[BLOCK_SIZE * BLOCK_SIZE];

				for (i = 0; i < BLOCK_SIZE * BLOCK_SIZE; i++) {
					ui_red[i] = (unsigned int)red[buf][i];
					ui_green[i] = (unsigned int)green[buf][i];
					ui_blue[i] = (unsigned int)blue[buf][i];
				}

				vector unsigned int *v_ui_red = (vector unsigned int *) ui_red;
				vector unsigned int *v_ui_green = (vector unsigned int *) ui_green;
				vector unsigned int *v_ui_blue = (vector unsigned int *) ui_blue;
			

				for (i = 0; i < BLOCK_SIZE * BLOCK_SIZE / 4; i++)
					v_luminances[i] = spu_convtf(v_ui_red[i], 0) * v_coef_red[0] + 
								spu_convtf(v_ui_green[i], 0) * v_coef_green[0] +
								spu_convtf(v_ui_blue[i], 0) * v_coef_blue[0];

				float _avg_luminance[4] ALIGNED = {0, 0, 0, 0};
				vector float * v_avg_luminance = (vector float *) _avg_luminance;

				for (i = 0; i < BLOCK_SIZE * BLOCK_SIZE / 4; i++)
					v_avg_luminance[0] += v_luminances[i];

				for (i = 0; i < 4; i++) {
					v_avg_luminance[0][i] /= (double)(BLOCK_SIZE * BLOCK_SIZE / 4);
					avg_luminance += v_avg_luminance[0][i];
				}

				avg_luminance /= 4.0;


				for (i = 0; i < BLOCK_SIZE * BLOCK_SIZE; i++) {
					curr_block.bitmap[i] =	((luminances[i] >= avg_luminance) ? 1 : 0);
					nr_ones += curr_block.bitmap[i];
				}
			}



			// -- medii in functie de luminanta
			curr_block.r1 = curr_block.g1 = curr_block.b1 = 0;
			curr_block.r2 = curr_block.g2 = curr_block.b2 = 0;

			float r1 = 0, g1 = 0, b1 = 0, r2 = 0, g2 = 0, b2 = 0;

			for (row = 0; row < BLOCK_SIZE; row++) {
				for (col = 0; col < BLOCK_SIZE; col++){
					if (curr_block.bitmap[row * BLOCK_SIZE + col]) {
							r1 += red[buf][row * BLOCK_SIZE + col];
							g1 += green[buf][row * BLOCK_SIZE + col];
							b1 += blue[buf][row * BLOCK_SIZE + col];
						} else {
							r2 += red[buf][row * BLOCK_SIZE + col];
							g2 += green[buf][row * BLOCK_SIZE + col];
							b2 += blue[buf][row * BLOCK_SIZE + col];			
						}
				}
			}

			
			r1 /= (float)nr_ones;
			g1 /= (float)nr_ones;
			b1 /= (float)nr_ones;
			r2 /= (float)(BLOCK_SIZE * BLOCK_SIZE - nr_ones);
			g2 /= (float)(BLOCK_SIZE * BLOCK_SIZE - nr_ones);
			b2 /= (float)(BLOCK_SIZE * BLOCK_SIZE - nr_ones);


			curr_block.r1 = (unsigned char)r1;
			curr_block.g1 = (unsigned char)g1;
			curr_block.b1 = (unsigned char)b1;

			curr_block.r2 = (unsigned char)r2;
			curr_block.g2 = (unsigned char)g2;
			curr_block.b2 = (unsigned char)b2;

			
			// trimit blockul inapoi spre ppu
			mfc_put((void *) &curr_block, (unsigned int)&(out_ccc.blocks[block_index - io_addrs.nr_spus * io_addrs.mod_dma]), sizeof(ccc_block_t), tag[buf], 0, 0);

			// astept sa faca trimiterea
			wait_tag(tag[buf]);


			block_index += io_addrs.nr_spus;
			buf = nxt_buf;
		}


		status = 1;
		spu_write_out_mbox(status);
	}

	status = 2;
	spu_write_out_mbox(status);

	status = spu_read_in_mbox();

	// eliberare tag-uri
	mfc_tag_release(tag[0]);
	mfc_tag_release(tag[1]);

	return 0;
}

