#include "list.h"
#include "stdlib.h"
#include "string.h"
#include "util.h"

vm_mapinfo_t *mapinfo_list;
int max_size;
int crt_size;

/* Sporici Dan / 335CB */

/*
 * adauga in lista map-ul dat alaturi de alti parametri necesari mai tarziu
 *
 * map - pointer catre map-ul dat in vm_alloc
 * num_pages - nr de pagini din acest map
 * num_frames - nr de cadre
 */
void add_mapinfo(vm_map_t *map, w_size_t num_pages, w_size_t num_frames)
{
	int i = 0;

	if (max_size - crt_size == 0) {
		max_size = max_size * 2 + 1;
		mapinfo_list = realloc(mapinfo_list,
					max_size * sizeof(vm_mapinfo_t));

		DIE(mapinfo_list == NULL, "realloc err");
	}

	crt_size++;

	mapinfo_list[crt_size-1].pmap = map;
	mapinfo_list[crt_size-1].num_pages = num_pages;
	mapinfo_list[crt_size-1].num_frames = num_frames;
	mapinfo_list[crt_size-1].prot_levels = malloc(sizeof(w_prot_t)
							* num_pages);

	mapinfo_list[crt_size-1].locations = malloc(sizeof(w_locations_t)
							* num_pages);
	DIE(mapinfo_list[crt_size-1].locations == NULL, "malloc err");

	mapinfo_list[crt_size-1].evac = malloc(sizeof(char) * num_pages);
	DIE(mapinfo_list[crt_size-1].evac == NULL, "malloc err");

	mapinfo_list[crt_size-1].ram_locations = malloc(sizeof(int)
							   * num_frames);
	DIE(mapinfo_list[crt_size-1].ram_locations == NULL, "malloc err");

	mapinfo_list[crt_size-1].swap_locations = malloc(sizeof(int)
							* num_pages);
	DIE(mapinfo_list[crt_size-1].swap_locations == NULL, "malloc err");

	mapinfo_list[crt_size-1].dirty = malloc(num_pages);
	DIE(mapinfo_list[crt_size-1].dirty == NULL, "malloc err");

	for (i = 0; i < num_pages; i++) {
		mapinfo_list[crt_size-1].prot_levels[i] = PROTECTION_NONE;
		mapinfo_list[crt_size-1].locations[i] = LOCATION_NONE;
	}

	memset(mapinfo_list[crt_size-1].ram_locations, -1, sizeof(int)
							* num_frames);
	memset(mapinfo_list[crt_size-1].swap_locations, -1, sizeof(int)
							* num_pages);
	memset(mapinfo_list[crt_size-1].dirty, 0, num_pages);
	memset(mapinfo_list[crt_size-1].evac, 0, num_pages);
}

/*
 * returns: dimensiunea listei
 */
w_size_t get_list_size(void)
{
	return crt_size;
}

/*
 * intoarce un pointer catre elementul cu indicele dat
 *
 * index - indice element cautat
 *
 * returns: pointer catre elem. din lista
 */
vm_mapinfo_t *get_element_at(int index)
{
	return &(mapinfo_list[index]);
}


/*
 * elibereaza memoria folosita
 */
void free_mapinfo_list(void)
{
	if (mapinfo_list != NULL) {
		free(mapinfo_list);
		mapinfo_list = NULL;
	}
}
