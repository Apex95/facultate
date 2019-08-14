/* SPORICI Dan - 315CB */

#include<stdio.h>
#include<stdlib.h>
#include "bsptree.h"
#include<math.h>

#define precision 0.001

TBspTree * createNode(NodeType type)
{
    TBspTree * treeNode = (TBspTree*)malloc(sizeof(TBspTree));

    if (treeNode == NULL)
        return NULL;

    treeNode->left = NULL;
    treeNode->right = NULL;
    treeNode->type = type;
    treeNode->index = -1;
    

    return treeNode;
}

// citeste si creeaza arborele pentru modul 1 de functionare
TBspTree * createTree(FILE * f, TBspTree ** mainNode)
{
    int  i = 0, l = 0, r = 0;
    fscanf(f, "%d %d %d", &i, &l, &r);

    TBspTree * crtNode = createNode(LINE);
    if (crtNode == NULL)
        return NULL;

    crtNode->index = i;

    if (*mainNode == NULL)
        *mainNode = crtNode;

    if (l == 1)
        crtNode->left = createTree(f, mainNode);

    if (r == 1)
        crtNode->right = createTree(f, mainNode);

    return crtNode;

}


// citeste datele de intrare (inclusiv arborele) 
TBspTree * readInput(FILE * f, int method, 
                                 TLine ** lines, int * n, 
                                 TPoint ** points, int * m, 
                                 TPoint ** qPoints, int * q)
{
    int i = 0;
 
    fscanf(f, "%d", n);
    *lines = (TLine*)malloc(sizeof(TLine) * (*n));

    if (*lines == NULL)
        return NULL;

    // n linii
    for (i = 0; i < *n; i++)
        fscanf(f, "%d %d %d", &((*lines)[i].a), 
                              &((*lines)[i].b), 
                              &((*lines)[i].c));
 
    fscanf(f, "%d", m);
    *points = (TPoint*)malloc(sizeof(TPoint) * (*m));

    if (*points == NULL)
        return NULL;

    // m puncte
    for (i = 0; i < *m; i++)
        fscanf(f, "%d %d", &((*points)[i].x), &((*points)[i].y));
   
    TBspTree * tree = NULL; 
    // arborele, daca se cere modul 1
    if (method == 1)
        createTree(f, &tree);


    fscanf(f, "%d", q);
    
    *qPoints = (TPoint*)malloc(sizeof(TPoint) * (*q));
    if (*qPoints == NULL)
        return NULL;
    // q puncte
    for (i = 0; i < *q; i++)
        fscanf(f, "%d %d", &((*qPoints)[i].x), &((*qPoints)[i].y));
    

    return tree;
}


// adauga frunze pentru fiecare nod ce contine o dreapta
void addLeaves(TBspTree * tree)
{
    if (tree == NULL)
        return;

    addLeaves(tree->left);
    addLeaves(tree->right);
    
    if (tree->left == NULL)
    {
        TBspTree * newNode = createNode(POINT);
        if (newNode == NULL)
            return;

        tree->left = newNode;
    }
    
    if (tree->right == NULL)
    {
        TBspTree * newNode = createNode(POINT);
        if (newNode == NULL)
            return;
        tree->right = newNode;
    }
}

// atribuie indicele dat ca parametru unei frunze
void setLeafIndex(TBspTree * tree, TLine lines[], int index, TPoint points[])
{
    if (tree == NULL)
        return;

    if (tree->type == POINT)
    {
        tree->index = index;
        return;
    }
    
    if (getPlane(lines[tree->index], points[index]) < 0) 
        setLeafIndex(tree->left, lines, index, points);
    
    if (getPlane(lines[tree->index], points[index]) > 0)
        setLeafIndex(tree->right, lines, index, points);  
}


// afiseaza arborele
void showTree(FILE * g, TBspTree * tree, TLine lines[], TPoint points[])
{
    if (tree == NULL)
        return;

     
    showTree(g, tree->left, lines, points);
    showTree(g, tree->right, lines, points);

    if (tree->type == LINE)  
        fprintf(g, "(%d,%d,%d)", lines[tree->index].a, 
                                 lines[tree->index].b,
                                 lines[tree->index].c);

    

    if (tree->type == POINT)
    {
        if (tree->index == -1) // nu exista un indice pt frunza asta=>null
            fprintf(g, "null");
        else
            fprintf(g, "((%d,%d)-%d)", points[tree->index].x,
                                       points[tree->index].y,
                                       tree->index);
    }

}

// obtine indicele corespunzator unuia dintre cele Q puncte
void getLeafIndex(FILE * g, TBspTree * tree, TLine lines[], TPoint iPoint)
{
    if (tree == NULL)
        return;

    if (tree->left == NULL && tree->right == NULL)
    {
        fprintf(g, "%d\n", tree->index);
        return;    
    }

    if (getPlane(lines[tree->index], iPoint) < 0) 
        getLeafIndex(g, tree->left, lines, iPoint);
    
    if (getPlane(lines[tree->index], iPoint) > 0)
        getLeafIndex(g, tree->right, lines, iPoint);
 
}

// insereaza o dreapta in arbore

TBspTree * applyLine(TBspTree * tree, TLine lines[], int index)
{
    if (tree == NULL)
    {
        tree = createNode(LINE);

        if (tree == NULL)
            return NULL;

        tree->index = index;
        return tree;
    }

    // daca pantele difera, linia taie si planul din stanga si cel din dreapta
    
    if (fabs(getSlope(lines[tree->index]) - getSlope(lines[index])) > precision)
    {
        // adauga un nod nou(frunza) corespunzator liniei
        if (tree->left == NULL)
            tree->left = applyLine(tree->left, lines, index);
        else
            applyLine(tree->left, lines, index);

        if (tree->right == NULL)
            tree->right = applyLine(tree->right, lines, index);
        else
            applyLine(tree->right, lines, index);
    }
    else // pt pante egale
    {
        // un factor (1 sau -1) cu ajutorul caruia determin 
        // daca linia ce trebuie adaugata trebuie sa vina in stanga
        // sau in dreapta (variaza in functie de semnul pantei)
        int slopeFactor = 1;
        if (getSlope(lines[tree->index]) < 0) 
            slopeFactor = -1;
        
        // compara valorile lui Y(x=0) pentru ambele drepte
        // fiind ambele paralele, linia cu un Y mai mare 
        // se afla in dreapta daca are panta negativa sau in stanga
        // daca are panta poztiva

        if (slopeFactor * lines[tree->index].c / (float)lines[tree->index].b < 
                 slopeFactor * lines[index].c / (float)lines[index].b)
        {
            if (tree->right == NULL)
                tree->right = applyLine(tree->right, lines, index);
            else
                applyLine(tree->right, lines, index);
        }
        else
        {
            if (tree->left == NULL)
                tree->left = applyLine(tree->left, lines, index); 
            else
                applyLine(tree->left, lines, index); 
        }
    }

    return tree;
}


// elibereaza memoria alocata pentru arborele de partitionare
void disposeTree(TBspTree ** tree)
{
    if (*tree == NULL)
        return;

    disposeTree(&(*tree)->left);
    disposeTree(&(*tree)->right);

    free(*tree);
    *tree = NULL;

}



