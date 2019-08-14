/* SPORICI Dan - 315CB */

typedef struct TProcess
{
    int pId;
    int pPriority;
    int pTime;
} TProcess;


int cmpOrdProc(void*, void*);
int getPid(void*);
int getProcSize();
void * setProcInfo(void *, int, int, int);
