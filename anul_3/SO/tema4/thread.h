#ifndef _thread_h

#define _thread_h
#include "so_scheduler.h"
#include <semaphore.h>

/* Sporici Dan / 335CB */

typedef enum {NEW, READY, RUNNING, WAITING, TERMINATED} state_t;

typedef struct so_thread_t
{
	so_handler *func;
	tid_t tid;
	unsigned int priority;
	state_t state;
	unsigned int time_quantum;
	unsigned int wait_device;
	sem_t sem;

} so_thread_t;

#endif
