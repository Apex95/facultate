#include "so_scheduler.h"
#include "thread_queue.h"
#include <stdlib.h>
#include <stdio.h>

/* Sporici Dan / 335CB */

unsigned int g_time_quantum, g_io;
bool is_init = FALSE;
unsigned int g_no_threads;

queue_elem_t *active_elem;


DECL_PREFIX int so_init(unsigned int time_quantum, unsigned int io)
{
	g_time_quantum = time_quantum;
	g_io = io;

	if (time_quantum <= 0 || io > SO_MAX_NUM_EVENTS)
		return SO_FAILURE;

	if (is_init == TRUE)
		return SO_FAILURE;

	is_init = TRUE;

	return SO_SUCCESS;
}

void schedule_next(void)
{
	queue_elem_t *last_elem = active_elem;

	active_elem = get_next_elem(last_elem);

	if (active_elem == NULL)
		return;


	if (last_elem == active_elem) {
		active_elem->thread->time_quantum = g_time_quantum;
		return;
	}

	active_elem->thread->state = RUNNING;
	active_elem->thread->time_quantum = g_time_quantum;

	sem_post(&(active_elem->thread->sem));

	if (last_elem != NULL && last_elem->thread->state != TERMINATED)
		sem_wait(&(last_elem->thread->sem));
}


void update_scheduler(void)
{
	if (active_elem == NULL)
		return;

	active_elem->thread->time_quantum--;

	if (active_elem->thread->time_quantum == 0 ||
			active_elem->thread->state == WAITING) {
		if (active_elem->thread->state != TERMINATED &&
			active_elem->thread->state != WAITING)
			active_elem->thread->state = READY;
		schedule_next();
	}
}


void start_thread(void *param)
{
	so_thread_t *thread = (so_thread_t *)param;

	thread->state = READY;

	sched_yield();
	DIE(sem_post(&(thread->sem)) != 0, "sem_post");

	DIE(sem_wait(&(thread->sem)) != 0, "sem_wait");

	active_elem->thread->func(thread->priority);

	active_elem->thread->state = TERMINATED;

	schedule_next();
}


DECL_PREFIX tid_t so_fork(so_handler *func, unsigned int priority)
{
	if (priority > SO_MAX_PRIO)
		return INVALID_TID;

	if (func == NULL)
		return INVALID_TID;


	so_thread_t *new_thread = calloc(1, sizeof(so_thread_t));

	DIE(new_thread == NULL, "calloc");

	new_thread->func = func;
	new_thread->priority = priority;

	DIE(sem_init(&(new_thread->sem), 0, 0) != 0, "sem_init");

	new_thread->state = NEW;
	new_thread->time_quantum = g_time_quantum;

	enqueue_thread(new_thread);

	DIE(pthread_create(&(new_thread->tid), NULL, (void *)start_thread,
				(void *)new_thread) != 0, "pthread_create");
	DIE(sem_wait(&(new_thread->sem)) != 0, "sem_wait");


	if (active_elem == NULL)
		schedule_next();
	else
		update_scheduler();

	return new_thread->tid;
}


DECL_PREFIX int so_wait(unsigned int io)
{
	if (io < 0 || io >= g_io)
		return SO_FAILURE;

	active_elem->thread->state = WAITING;
	active_elem->thread->wait_device = io;

	update_scheduler();

	return SO_SUCCESS;
}

DECL_PREFIX int so_signal(unsigned int io)
{
	if (io < 0 || io >= g_io)
		return SO_FAILURE;

	int n = wake_elems_up(io);

	update_scheduler();

	return n;
}

DECL_PREFIX void so_exec(void)
{
	update_scheduler();
}

DECL_PREFIX void so_end(void)
{
	is_init = FALSE;

	if (thread_queue == NULL)
		return;

	queue_elem_t *crt_elem = thread_queue;

	do {
		DIE(pthread_join(crt_elem->thread->tid, NULL) != 0,
						"pthread_join");
		crt_elem = crt_elem->next;

	} while (crt_elem != thread_queue);

	crt_elem = thread_queue->next;

	while (crt_elem != thread_queue) {
		crt_elem = crt_elem->next;
		DIE(sem_destroy(&(crt_elem->thread->sem)) != 0, "sem_destroy");
		free(crt_elem->prev->thread);
		free(crt_elem->prev);
	}

	free(thread_queue->thread);
	free(thread_queue);
}
