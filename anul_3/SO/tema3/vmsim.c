#include "stdio.h"
#include "vmsim.h"
#include "stdlib.h"
#include "util.h"
#include "list.h"
#include <string.h>

/* Sporici Dan / 335CB */


#define FIND_SWAP_SPACE(j2) do {	\
	for (j2 = 0; j2 < crt_mapinfo->num_pages; j2++)	\
		if (crt_mapinfo->swap_locations[j2] == -1)	\
			break;	\
	} while (0)

#define REMOVE_RAM_PAGE(j2) do {	\
	for (j2 = 0; j2 < crt_mapinfo->num_frames; j2++)	\
		if (crt_mapinfo->ram_locations[j2] == r_page) \
			crt_mapinfo->ram_locations[j2] = -1;	\
	} while (0)

#define FIND_SWAP_PAGE(j3) do {		\
	for (j3 = 0; j3 < get_element_at(i)->num_pages; j3++)	\
		if (get_element_at(i)->swap_locations[j3] == page) \
			break;	\
	} while (0)


void page_fault_handler(int signum, siginfo_t *info, void *context)
{
	if (signum != SIGSEGV)
		return;

	w_ptr_t addr = (w_ptr_t)info->si_addr;

	int i = 0;
	w_size_t page_size = w_get_page_size();

	vm_mapinfo_t *crt_mapinfo;

	for (i = 0; i < get_list_size(); i++) {
		crt_mapinfo = get_element_at(i);
		if (addr - crt_mapinfo->pmap->start >= 0 &&
			addr - crt_mapinfo->pmap->start
			<= crt_mapinfo->num_pages * page_size) {

			int page = (addr-crt_mapinfo->pmap->start) / page_size;

			/* -- daca nu e deja in ram, incerc sa o mapez aici */
			if (crt_mapinfo->locations[page] == LOCATION_NONE) {

				int j = 0;

				/* caut loc disponibil in ram */
				for (j = 0; j < crt_mapinfo->num_frames; j++)
					if (crt_mapinfo->ram_locations[j] == -1)
						break;

				/* swap out prima pagina */
				if (j == crt_mapinfo->num_frames) {
					int j2 = 0;


					/* caut loc disponibil in swap */
					FIND_SWAP_SPACE(j2);

					DIE(j2 == crt_mapinfo->num_pages,
							"no space in swap");


					/* -- mutare ram -> swap */
					int r_page =
						crt_mapinfo->ram_locations[0];


					w_handle_t _h;

					_h = crt_mapinfo->pmap->swap_handle;

					/* copiez in fisierul de swap */
					if (crt_mapinfo->dirty[r_page] == 1 ||
					(crt_mapinfo->dirty[r_page] == 0 &&
					crt_mapinfo->evac[r_page] == 0)) {

						DIE(w_set_file_pointer(_h,
						j2*page_size) == FALSE,
						"w_set_pointer err");

						DIE(w_write_file(_h,
						crt_mapinfo->pmap->start +
						r_page*page_size, page_size)
						== FALSE, "w_write err");
					}

					DIE(munmap(crt_mapinfo->pmap->start +
					r_page*page_size, page_size) == -1,
					"munmap err");

					w_ptr_t addr;

					addr = mmap(crt_mapinfo->pmap->start +
						r_page*page_size, page_size,
						PROT_NONE,
						MAP_SHARED | MAP_FIXED,
						_h, j2 * page_size);

					DIE(addr == MAP_FAILED, "mmap err");

					crt_mapinfo->prot_levels[r_page] =
							PROTECTION_NONE;

					crt_mapinfo->evac[r_page] = 1;
					crt_mapinfo->locations[r_page] =
							LOCATION_SWAP;

					crt_mapinfo->swap_locations[j2] =
							r_page;

					/* sterg maparea din ram */
					REMOVE_RAM_PAGE(j2);

					j = 0;
				}

				DIE(munmap(crt_mapinfo->pmap->start +
					page*page_size, page_size) == -1,
					"munmap err");

				w_ptr_t addr;

				addr = mmap(crt_mapinfo->pmap->start +
						page*page_size,
						page_size,
						PROT_READ | PROT_WRITE,
						MAP_SHARED | MAP_FIXED,
						crt_mapinfo->pmap->ram_handle,
						j * page_size);

				DIE(addr == MAP_FAILED, "mmap err");

				memset(crt_mapinfo->pmap->start +
						page*page_size, 0, page_size);

				DIE(w_protect_mapping(crt_mapinfo->pmap->start +
				page*page_size, 1, PROTECTION_NONE) == FALSE,
				"w_prot err");

				crt_mapinfo->prot_levels[page] =
							PROTECTION_NONE;
				crt_mapinfo->locations[page] = LOCATION_RAM;
				crt_mapinfo->ram_locations[j] = page;
			}

			/* -- ajustare flaguri de protectie */
			if (crt_mapinfo->prot_levels[page] == PROTECTION_NONE) {

				DIE(w_protect_mapping(crt_mapinfo->pmap->start +
				page*page_size, 1, PROTECTION_READ) == FALSE,
				"w_prot err");

				crt_mapinfo->prot_levels[page] =
							PROTECTION_READ;

				return;
			}

			if (crt_mapinfo->prot_levels[page] == PROTECTION_READ) {

				DIE(w_protect_mapping(crt_mapinfo->pmap->start +
				page*page_size, 1, PROTECTION_WRITE) == FALSE,
				"w_prot err");

				crt_mapinfo->prot_levels[page] =
							PROTECTION_WRITE;
				crt_mapinfo->dirty[page] = 1;

				return;
			}
		}
	}
}

w_exception_handler_t original_handler;

FUNC_DECL_PREFIX w_boolean_t vmsim_init(void)
{
	w_boolean_t res;

	res = w_get_current_exception_handler(&original_handler);
	if (res == FALSE)
		return res;

	return w_set_exception_handler(page_fault_handler);
}

FUNC_DECL_PREFIX w_boolean_t vmsim_cleanup(void)
{
	free_mapinfo_list();
	return w_set_exception_handler(original_handler);
}

FUNC_DECL_PREFIX w_boolean_t vm_alloc(w_size_t num_pages, w_size_t num_frames,
				      vm_map_t *map)
{
	if (num_pages < num_frames)
		return FALSE;

	char ram_name[] = "RAMXXXXXX", swap_name[] = "SWAPXXXXXX";

	w_size_t page_size = w_get_page_size();

	w_ptr_t addr = mmap(NULL, num_pages * page_size, PROT_NONE,
					MAP_SHARED | MAP_ANONYMOUS, 0, 0);

	DIE(addr == MAP_FAILED, "mmap err");

	map->start = addr;

	w_handle_t ram_handle = mkstemp(ram_name);
	w_handle_t swap_handle = mkstemp(swap_name);

	DIE(ram_handle == -1 || swap_handle == 1, "mkstemp err");



	DIE(ftruncate(ram_handle, num_frames * page_size) == -1, "ftrunc err");
	DIE(ftruncate(swap_handle, num_pages * page_size) == -1, "ftrunc err");

	map->ram_handle = ram_handle;
	map->swap_handle = swap_handle;

	add_mapinfo(map, num_pages, num_frames);

	return TRUE;
}


FUNC_DECL_PREFIX w_boolean_t vm_free(w_ptr_t start)
{
	if (start == NULL)
		return FALSE;

	w_size_t list_size = get_list_size();
	w_size_t page_size = w_get_page_size();
	int i = 0;


	for (i = 0; i < list_size; i++) {
		vm_mapinfo_t *crt_mapinfo = get_element_at(i);

		if (crt_mapinfo->pmap->start == start) {
			DIE(munmap(start,
				crt_mapinfo->num_pages * page_size) == -1,
				"munmap err");

			w_close_file(crt_mapinfo->pmap->ram_handle);
			w_close_file(crt_mapinfo->pmap->swap_handle);

			free(crt_mapinfo->prot_levels);
			free(crt_mapinfo->locations);
			free(crt_mapinfo->evac);
			free(crt_mapinfo->ram_locations);
			free(crt_mapinfo->swap_locations);
			free(crt_mapinfo->dirty);
		}
	}

	return TRUE;
}

