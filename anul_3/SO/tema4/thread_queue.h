#ifndef _thread_queue_h
#define _thread_queue_h

#include "thread.h"

/* Sporici Dan / 335CB */

typedef struct queue_elem_t
{
	so_thread_t *thread;
	struct queue_elem_t *next;
	struct queue_elem_t *prev;
} queue_elem_t;

extern queue_elem_t *thread_queue;
void enqueue_thread(so_thread_t *thread);
queue_elem_t *get_next_elem(queue_elem_t *last_elem);
int wake_elems_up(unsigned int io);


#endif
