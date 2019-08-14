/* SPORICI Dan - 315CB */

#ifndef _INCLUDE_MISCS
#define _INCLUDE_MISCS
#include "miscs.h"
#endif

typedef enum {LINE=1, POINT=2} NodeType;

typedef struct TBspTree
{
    struct TBspTree * left;
    struct TBspTree * right;
    int index;
    NodeType type;
} TBspTree;

TBspTree * createNode(NodeType);
TBspTree * createTree(FILE *, TBspTree **);
TBspTree * readInput(FILE *, int, TLine **, int *, 
                                  TPoint **, int *, 
                                  TPoint **, int *);
void addLeaves(TBspTree *);
void setLeafIndex(TBspTree *, TLine *, int, TPoint *);
void showTree(FILE *, TBspTree *, TLine *, TPoint *);
void getLeafIndex(FILE *, TBspTree *, TLine *, TPoint);
TBspTree * applyLine(TBspTree *, TLine *, int);
void disposeTree(TBspTree**);

