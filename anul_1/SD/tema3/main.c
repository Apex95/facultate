/* SPORICI Dan - 315CB */

#include<stdio.h>
#include<stdlib.h>
#include "bsptree.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
        return -1;

    int method = atoi(argv[1]);
    char * inputFile = argv[2];
    char * outputFile = argv[3];

    // vectori pt dreptele si punctele date
    TLine * lines = NULL; int n = 0;
    TPoint * points = NULL; int m = 0;
    TPoint * qPoints = NULL; int q = 0;

    FILE * f = fopen(inputFile, "r");

    TBspTree * tree = readInput(f, method, &lines, &n, 
                                           &points, &m,
                                           &qPoints, &q);
    fclose(f);

    int i = 0;
    
    // creeaza arborele pt modul 2 de functionare
    // insereaza nodurile coresp. dreptelor
    
    if (method == 2)
    {
        for (i = 0; i < n; i++)
            if (tree == NULL)
                tree = applyLine(tree, lines, i);
            else
                applyLine(tree, lines, i);
        
    }

    // la arborele creat se adauga pt fiecare nod 2 frunze
    addLeaves(tree);

    // atribuie indicele corespunzator fiecarei frunze 
    for (i = 0; i < m; i++)
        setLeafIndex(tree, lines, i, points);


    // afisare
    FILE * g = fopen(outputFile, "w");
    
    showTree(g, tree, lines, points); 
    fprintf(g, "\n");
    for (i = 0; i < q; i++)
        getLeafIndex(g, tree, lines, qPoints[i]); 
    
    fclose(g); 
    
    //free...
    disposeTree(&tree);
    free(qPoints);
    free(points);
    free(lines);     
    return 0;
}
