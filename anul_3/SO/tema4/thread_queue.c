#include "thread_queue.h"
#include <stdlib.h>
#include <stdio.h>

/* Sporici Dan / 335CB */

queue_elem_t *thread_queue;

void enqueue_thread(so_thread_t *thread)
{
	queue_elem_t *new_elem = calloc(1, sizeof(queue_elem_t));

	DIE(new_elem == NULL, "calloc");

	new_elem->thread = thread;
	new_elem->next = new_elem;
	new_elem->prev = new_elem;

	if (thread_queue == NULL) {
		thread_queue = new_elem;
		return;
	}

	queue_elem_t *crt_elem = thread_queue;

	do {
		if (crt_elem->thread->priority >= new_elem->thread->priority)
			crt_elem = crt_elem->next;
		else
			break;

	} while (crt_elem != thread_queue);

	crt_elem = crt_elem->prev;

	new_elem->next = crt_elem->next;
	new_elem->next->prev = new_elem;

	new_elem->prev = crt_elem;
	crt_elem->next = new_elem;

	if (new_elem->thread->priority > crt_elem->thread->priority)
		thread_queue = new_elem;
}

queue_elem_t *get_next_elem(queue_elem_t *last_elem)
{
	queue_elem_t *lim_elem = last_elem != NULL ? last_elem : thread_queue;
	queue_elem_t *crt_elem = lim_elem;
	queue_elem_t *crt_elem2 = thread_queue;

	queue_elem_t *max_prio_elem = NULL;


	crt_elem = lim_elem->next;

	do {
		if (crt_elem2->thread->state == READY &&
		crt_elem2->thread->priority > lim_elem->thread->priority)
			return crt_elem2;


		if (crt_elem->thread->state == READY &&
			(max_prio_elem == NULL ||
		max_prio_elem->thread->priority < crt_elem->thread->priority)){
			max_prio_elem = crt_elem;

			if (crt_elem2->thread->priority <
				lim_elem->thread->priority)
				break;
		}

		crt_elem = crt_elem->next;
		crt_elem2 = crt_elem2->next;

	} while (crt_elem != lim_elem);


	if (max_prio_elem == NULL ||
		(lim_elem->thread->priority > max_prio_elem->thread->priority &&
		lim_elem->thread->state == READY))
		return lim_elem;
	else
		return max_prio_elem;


	return NULL;
}


int wake_elems_up(unsigned int io)
{
	queue_elem_t *crt_elem = thread_queue;

	int nOfAwakenedElems = 0;

	do {
		if (crt_elem->thread->state == WAITING &&
			crt_elem->thread->wait_device == io) {
			crt_elem->thread->state = READY;
			nOfAwakenedElems++;
		}

		crt_elem = crt_elem->next;
	} while (crt_elem != thread_queue);

	return nOfAwakenedElems;
}
