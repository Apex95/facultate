/* SPORICI Dan - 315CB */

#define CMD_SUCCESS 1
#define CMD_FAIL 0

#ifndef _PROCESS_HEADER_
#include "process.h"
#define _PROCESS_HEADER_
#endif

typedef struct TQueue
{
    struct TQueue * addr;
    void * process;
} TQueue;

int qINSERT(void *, TQueue **);
int qPOP(void *, TQueue **);
void showQueue(FILE *, TQueue *);
void disposeQueue(TQueue **);
