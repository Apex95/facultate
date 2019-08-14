#ifndef LIST_H_
#define LIST_H_

#include "vmsim.h"

typedef enum {
	LOCATION_NONE,
	LOCATION_RAM,
	LOCATION_SWAP
} w_locations_t;

typedef struct vm_mapinfo {
	vm_map_t *pmap; /* pointer catre map */
	w_size_t num_pages;
	w_size_t num_frames;
	w_prot_t *prot_levels; /* nivelul de protectie pt fiecare pagina */
	w_locations_t *locations; /* locatia fiecarei pagini */
	char *evac; /* daca a fost evacuata sau nu o pagina anume */
	int *ram_locations; /* indicele (in ram) ce corespunde paginii */
	int *swap_locations; /* indicele (in swap) pt pagina */
	char *dirty; /* evidenta paginilor dirty */
} vm_mapinfo_t;




void add_mapinfo(vm_map_t *map, w_size_t num_pages, w_size_t num_frames);
vm_mapinfo_t *get_element_at(int index);
w_size_t get_list_size(void);
void free_mapinfo_list(void);

#endif
