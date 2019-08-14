/* SPORICI Dan - 315CB */

#include<stdio.h>
#include "table.h"
#include<string.h>

#define LINE_LENGTH 200


int execCommand(FILE * g, Tbucket ** table, size_t tableSize, char * s)
{
    if (strlen(s) < 2)
        return 0;

    if (s[strlen(s) - 1] == '\n')
        s[strlen(s) - 1] = '\0';

    // imparte linia din fisier in 3 variabile
    // [comanda] [argument1] [argument2]
    char * cmd = strtok(s, " ");
    char * arg1 = strtok(NULL, " ");
    char * arg2 = strtok(NULL, " ");

    // apeleaza comanda respectiva

    if (strcmp(cmd, "put") == 0)
    {
        addToTable(table, tableSize, arg1, arg2);
        return 1;
    }

    if (strcmp(cmd, "get") == 0)
    {
        getValue(g, table, tableSize, arg1, 0);
        return 1;
    }

    if (strcmp(cmd, "remove") == 0)
    {
        removeFromTable(table, tableSize, arg1);
        return 1;
    }

    if (strcmp(cmd, "find") == 0)
    {
        
        findKey(g, table, tableSize, arg1);
        return 1;
    }

    if (strcmp(cmd, "print") == 0)
    {
        dumpTable(g, table, tableSize, -1);
        return 1;
    }

    if (strcmp(cmd, "print_bucket") == 0)
    {
        dumpTable(g, table, tableSize, atoi(arg1));
        return 1;
    }

    // return 0 - nu exista comanda
    return 0;
}


int main(int argc, char * argv[])
{
    // se asteapta 3 parametri din linia de comanda
    if (argc != 4)
        return -1;

    // dimensiunea tabelului
    size_t tableSize = atoi(argv[1]);

    // aloca spatiu pentru tabel(vector)
    Tbucket ** table = createTable(tableSize);

    // nu se poate aloca tabelul
    if (!table)
        return -1;

    // vector in care se citeste linia de comanda (din fisier)
    char line[LINE_LENGTH+1];

    // fisiere date ca parametri
    FILE * f = fopen(argv[2], "r");
    FILE * g = fopen(argv[3], "w");
   
    // cat timp exista comenzi in fisier, le executa
    while (fgets(line, LINE_LENGTH, f) != NULL)
        execCommand(g, table, tableSize, line);
    
    fclose(f);
    fclose(g);

    // elibereaza memoria alocata pt tabel + buckets
    disposeAll(table, tableSize);

    return 0;
}
