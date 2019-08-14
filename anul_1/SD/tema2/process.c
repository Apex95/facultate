/* SPORICI Dan - 315CB */

#include "process.h"

// functii ce tin strict de tipul TProcess (pt listele generice)

// compara 2 procese dupa prioritate iar mai apoi dupa timp
// conform enuntului (returneaza -1 daca ordinea e corecta)
int cmpOrdProc(void * v1, void * v2)
{
    TProcess * p1 = (TProcess *)v1;
    TProcess * p2 = (TProcess *)v2;

    if (p1->pPriority < p2->pPriority)
        return -1;

    if (p1->pPriority == p2->pPriority)
    {
        if (p1->pTime > p2->pTime)
            return -1;

        if (p1->pTime < p2->pTime)
            return 1;
    }

    if (p1->pPriority > p2->pPriority)
        return 1;

    return 0;
}

// intoarce pId-ul unui proces
int getPid(void * v)
{
    return ((TProcess*)v)->pId;
}

// dimensiunea structurii TProcess (folosita la malloc-uri)
int getProcSize()
{
    return sizeof(TProcess);
}

// setaza pId-ul, prioritatea si timpul unui nou proces
void * setProcInfo(void * p, int pId, int pPriority, int pTime)
{
    ((TProcess*)p)->pId = pId;
    ((TProcess*)p)->pPriority = pPriority;
    ((TProcess*)p)->pTime = pTime;

    return (void*)p;
}

