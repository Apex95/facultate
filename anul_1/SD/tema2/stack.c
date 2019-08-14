/* SPORICI Dan - 315CB */

#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "stack.h"

// functia push pt stive
int sPUSH(void * p, TStack ** start)
{
    TStack * cell = (TStack*)malloc(sizeof(TStack));
    
    if (cell == NULL)
        return CMD_FAIL;

    void * proc = malloc(getProcSize());

    if (proc == NULL)
    {
        free(cell);
        return CMD_FAIL;
    }

    // copiaza datele procesului din zona de memorie indicata de parametru
    memcpy(proc, p, getProcSize());

    // adauga procesul in stiva
    cell->process = proc;
    cell->addr = *start;
    *start = cell;

    return CMD_SUCCESS; 
}


// functia pop
int sPOP(void * p, TStack ** start)
{
    if (*start == NULL)
        return CMD_FAIL;
    
    TStack * cell = *start;
    *start = (*start)->addr;
    
    // copiaza informatiile despre procesul aflat in stiva in zona
    // indicata de pointerul 'p'
    memcpy(p, cell->process, getProcSize());

    // elimina procesul din stiva
    free(cell->process);
    free(cell);

    return CMD_SUCCESS;
}

// afiseaza stiva data ca parametru
void showStack(FILE * g, TStack * start)
{
    while (start != NULL)
    {
        fprintf(g, "%d ", getPid(start->process));
        start = start->addr;
    }
}

// elibereaza zona de memorie folosita de stiva
void disposeStack(TStack ** Pstart)
{
    TStack * p = NULL;

    while (*Pstart != NULL)
    {
        p = *Pstart;
        *Pstart = (*Pstart)->addr;

        free(p->process);
        free(p);
    }

    *Pstart = NULL;
}
