/* SPORICI Dan - 315CB */


#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include "table.h"

// functii pt listele generice



// returneaza o cheie din lista data
char * keyOf(Tbucket * p)
{
   return ((TbucketCell *)p->info)->key; 
}

// returneaza o valoare din lista
char * valueOf(Tbucket * p)
{
   return ((TbucketCell *)p->info)->value; 
}

// creeaza un element in care sunt retinute campurile (key, value)
// pt lista generica
void * createInfoCell(char * key, char * value)
{
    TbucketCell * p = (TbucketCell*)malloc(sizeof(TbucketCell));
    
    if (!p)
        return NULL;

    p->key = strdup(key);
    p->value = strdup(value);
    
    return (void*)p; 
}



// functii ce tin de tabel

// creeaza tabelul
Tbucket ** createTable(size_t tableSize)
{
    Tbucket ** hashTable = (Tbucket **)malloc(sizeof(Tbucket) * tableSize);

    if (!hashTable)
        return NULL;

    int i = 0;

    for(i = 0; i < tableSize; i++)
        hashTable[i] = NULL;
    
    return hashTable;
}

// calculeaza hashul corespunzator unei chei
HASH computeHash(char * key, size_t tableSize)
{
    int keyLength = strlen(key);
    int i = 0;
    HASH currentHash = 0; 

    for (i = 0; i < keyLength; i++)
        currentHash += key[i];

    return currentHash % tableSize;
}

// preia valoarea corespunzatoare unei chei din tabel
char * getValue(FILE * g, Tbucket ** table, 
                size_t tableSize, char * key, int suppress)
{
    // 'suppress' indica daca nu se doreste scrierea rezultatului in fisier

    HASH h = computeHash(key, tableSize);
    Tbucket * p = table[h];
    
    while (p != NULL)  // itereaza prin lista
        if (strcmp(keyOf(p), key) == 0)  //compara fiecare cheie
        {
            if (!suppress) 
                fprintf(g, "%s\n", valueOf(p)); 
            return valueOf(p); //intoarce valoarea corespunzatoare cheii
        }
        else
            p = p->addr; 

    if (!suppress) 
        fprintf(g, "NULL\n");

    return NULL;
}

// cauta o cheie, daca exista => true altfel false
int findKey(FILE * g, Tbucket ** table, size_t tableSize, char * key)
{
    if (getValue(g, table, tableSize, key, 1) == NULL)
    {
        fprintf(g, "False\n");
        return 0;
    }
    else
    {
        fprintf(g, "True\n");
        return 1;
    }
}

// adauga un element in tabel
int addToTable(Tbucket ** table, size_t tableSize, char * key, char * value)
{
    // daca deja exista, nu mai adauga nimic
    if (getValue(NULL, table, tableSize, key, 1) != NULL)
        return 0;


    HASH h = computeHash(key, tableSize);
    
    // creeaza un nou element pt bucketul respectiv    
    Tbucket * b = (Tbucket *)malloc(sizeof(Tbucket));

    // nu se poate aloca un nou element pt bucket ?
    if (!b)
        return 0;

    b->info = (void*)createInfoCell(key, value);

    // sau nu se poate aloca o noua celula (key,value)
    if (!b->info)
    {
        disposeElement(b);
        return 0;
    }

    b->addr = NULL;

    // daca bucketul respectiv nu contine niciun item, asta e primul
     if (table[h] == NULL)
    {
        table[h] = b;
        return 1;
    }
    
    Tbucket * p = table[h];

    // caz special in care elementul nou adaugat trebuie sa fie chiar
    // pe prima pozitie din bucket
    if (strcmp(keyOf(p), key) > 0)
    {
        table[h] = b;
        b->addr = p;
        return 1;
    }

    // altfel se parcurge bucketul pana la locatia noului element si se
    // face inserarea in lista
    while (p->addr != NULL && strcmp(keyOf(p->addr), key) < 0)
        p = p->addr;

    
    b->addr = p->addr;
    p->addr = b;
    
    return 1;

}

// sterge un element dat din tabel
int removeFromTable(Tbucket ** table, size_t tableSize, char * key)
{
    // daca nu exista...return
    if (getValue(NULL, table, tableSize, key, 1) == NULL)
        return 0;
    

    HASH h = computeHash(key, tableSize);
    Tbucket * p = table[h];

    // caz separat cand trebuie sters chiar primul element din tabel
    if (strcmp(keyOf(p), key) == 0)
    {
        table[h] = table[h]->addr;
        disposeElement(p);
        return 1;
    }
    
    // se cauta elementul in tabel si se sterge
    while (p->addr != NULL && strcmp(keyOf(p->addr), key) != 0)
        p = p->addr;

    
    Tbucket * q = p->addr;
    p->addr = q->addr;
    disposeElement(q);

    return 1;
}

// elibereaza memoria alocata de un element din tabel
void disposeElement(Tbucket * p)
{
    if (p->info != NULL)
    {
        free(keyOf(p));
        free(valueOf(p));
    
        free(p->info);
    }
    free(p);
}

// elibereaza memoria folosita de tot tabelul de hash
void disposeAll(Tbucket ** table, size_t tableSize)
{
    int i = 0;
    Tbucket * p = NULL;
    
    for (i = 0; i < tableSize; i++)
    {
        while (table[i] != NULL)
        {
            p = table[i];
            table[i] = table[i]->addr;
            disposeElement(p);
        }
    }
    
    
    free(table);
    table = NULL;
}


// afiseaza ce e in tabel
// index == -1, afiseaza tot tabelul (print)
// index != -1, afiseaza doar bucketul cerut (print_bucket)
void dumpTable(FILE * g, Tbucket ** table, size_t tableSize, int index)
{
    // se cere afisarea unui bucket
    if (index != -1) 
    {
        if (index >= tableSize)  // caz in care indicele cerut e prea mare
            return;

        Tbucket * p = table[index];
        
        if (p == NULL)
            fprintf(g, "VIDA\n");
        else
        {
            while (p != NULL)
            {
                fprintf(g, "%s ", valueOf(p));
                p = p->addr;
            }
            fprintf(g, "\n");
        }
    }
    else // se cere afisarea intregului tabel
    {
        int i = 0;
        Tbucket * p = NULL;
        
        for (i = 0; i < tableSize; i++)
        {
            if (table[i] == NULL)
                continue;

            p = table[i];
            fprintf(g, "%d: ", i);

            while (p != NULL)
            {
                fprintf(g, "%s ", valueOf(p));
                p = p->addr;
            }

            fprintf(g, "\n");
        }
    }
}
