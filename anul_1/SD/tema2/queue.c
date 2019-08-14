/* SPORICI Dan - 315CB */

#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "queue.h"


// inserare ordonata in coada de prioritati
int qINSERT(void * p, TQueue ** start)
{
    TQueue * cell = (TQueue*)malloc(sizeof(TQueue));

    if (cell == NULL)
        return CMD_FAIL;

    void * proc = malloc(getProcSize());

    if (proc == NULL)
    {
        free(cell);
        return CMD_FAIL;
    }

    // copiaza datele procesului 'p' dat ca parametru
    memcpy(proc, p, getProcSize());

    cell->process = proc;
    
    TQueue * s = *start;

    // cazul in care noul proces este cel mai din stanga in coada (ultimul)
    if (s != NULL && cmpOrdProc(s->process, proc) == 1)
    {
        cell->addr = s;
        *start = cell;
        
        return CMD_SUCCESS;
    }


    // altfel, se incearca incadrarea noului proces intre alte 2 procese
    // din coada 
    while (s != NULL && s->addr != NULL && cmpOrdProc(s->addr->process, proc) == -1)
    {
        s = s->addr;
    }

    // adauga procesul in coada
    if (s != NULL)
    {
        cell->addr = s->addr;
        s->addr = cell;

        return CMD_SUCCESS;
    }
    else
    {
        // s == NULL => noul proces este primul din coada de prioritati
        cell->addr = NULL;
        *start = cell;  

        return CMD_SUCCESS;  
    }
}


// extrage primul proces din coada (cu prioritatea cea mai mare si lansat
// cat mai devreme)
int qPOP(void * p, TQueue ** start)
{
    TQueue * s = *start;

    if (s == NULL)
        return CMD_FAIL;

    // caz special daca e doar 1 proces
    if (s->addr == NULL)
    {
        *start = NULL;
        memcpy(p, s->process, getProcSize());

        // elimina procesul din coada
        free(s->process);
        free(s);

        return CMD_SUCCESS;
    }

    // pentru 2 sau mai multe procese
    while (s->addr->addr != NULL)
        s = s->addr;

    // copiaza datele procesului in adresa indicata de pointerul 'p'
    memcpy(p, s->addr->process, getProcSize());
    
    // elimina procesul din coada
    free(s->addr->process);
    free(s->addr);

    s->addr = NULL;

    return CMD_SUCCESS;
}

// afisarea cozii (are legaturile invers)
void showQueue(FILE * g, TQueue * start)
{
    if (start == NULL)
        return;
    else
    {
        showQueue(g, start->addr);
        fprintf(g, "%d ", getPid(start->process));
    }
}

// functia de eliberare a memoriei
void disposeQueue(TQueue ** Pstart)
{
    TQueue * p = NULL;

    while (*Pstart != NULL)
    {
        p = *Pstart;
        *Pstart = (*Pstart)->addr;

        free(p->process);
        free(p);
    }

    *Pstart = NULL;

}


