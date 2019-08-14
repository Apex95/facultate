/* SPORICI Dan - 315CB */

#define CMD_SUCCESS 1
#define CMD_FAIL 0

#ifndef _PROCESS_HEADER_
#include "process.h"
#define _PROCESS_HEADER_
#endif

typedef struct TStack
{
    struct TStack * addr;
    void * process;
} TStack;

int sPUSH(void *, TStack **);
int sPOP(void *, TStack **);
void showStack(FILE *, TStack *);
void disposeStack(TStack **);  
