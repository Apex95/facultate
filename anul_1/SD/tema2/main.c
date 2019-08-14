/* SPORICI Dan - 315CB */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "stack.h"
#include "queue.h"


#define MAX_CMD_SIZE 50 // lungimea maxima unei comenzi din fisier

// scrie in fisier outputul cerut dupa fiecare pas
void showOutput(FILE * g, TStack * stacks[], TQueue * queue1, 
                                    int nOfEvents, int time)
{
    fprintf(g, "%d\n", time);
    showQueue(g, queue1);  // coada de prioritati

    fprintf(g, "\n");

    int i = 0;

    // vectorul de stive
    for (i = 0; i < nOfEvents; i++)
    {
        if (stacks[i] == NULL)
            continue;

        fprintf(g, "%d: ", i);
        showStack(g, stacks[i]);
        fprintf(g, "\n");
    }

    fprintf(g, "\n");
}


void execCommand(FILE * g, char * s, TStack * stacks[], TQueue ** Pqueue1, 
                                    int nOfEvents, int time)
{
    // ignora ultimul caracter newline (\n)
    if (strlen(s) < 2)
        return;

    // elimina caracterul \n din capatul liniei
    if (s[strlen(s) - 1] == '\n')
        s[strlen(s) - 1] = '\0';
    
    // imparte linia in comanda, argument1 si argument2
    char * cmd = strtok(s, " ");
    char * arg1 = strtok(NULL, " ");
    char * arg2 = strtok(NULL, " ");

    // folosit pentru inserarea / extragerea proceselor
    void * proc = malloc(getProcSize());
    

    if (strcmp(cmd, "start") == 0)
    {
        // seteaza informatiile pt noul proces
        proc = setProcInfo(proc, atoi(arg1), atoi(arg2), time);    
        qINSERT(proc, Pqueue1); // il insereaza in coada de prioritati
    }
    
    if (strcmp(cmd, "wait") == 0)
    {
        // coada auxiliara in care se muta procesele ce se afla
        // inaintea procesului ce trebuie trecut in asteptare (pt parcurgere)
        TQueue * aux = NULL; 

        int _arg2 = atoi(arg2);
        
        // muta procesele din coada de prioritati in aux pana cand se
        // intalneste procesul cu pId-ul cerut
        while (qPOP(proc, Pqueue1) == CMD_SUCCESS)
        {
            if (getPid(proc) == _arg2)
            {
                // procesul gasit e inserat in stiva corespunzatoare event-ului
                sPUSH(proc, &stacks[atoi(arg1)]);
                break;
            }
           
            qINSERT(proc, &aux);
        }
        
        // muta procesele din aux inapoi in coada de prioritati
        while (qPOP(proc, &aux) == CMD_SUCCESS)
        {
            qINSERT(proc, Pqueue1);
        }
    }
    
    if (strcmp(cmd, "event") == 0)
    {
        int _arg1 = atoi(arg1);

        // muta toate procesele din stiva pt care a avut loc un event
        // inapoi in coada
        while (sPOP(proc, &stacks[_arg1]) == CMD_SUCCESS)
            qINSERT(proc, Pqueue1);
    }

    if (strcmp(cmd, "end") == 0)
    {
        // alta coada auxiliara
        TQueue * aux = NULL; 

        int _arg1 = atoi(arg1);
        
        // la fel ca la 'wait', muta procesele in aux si inapoi
        // exceptie facand procesul ce trebuie terminat
        while (qPOP(proc, Pqueue1) == CMD_SUCCESS)
        {
            if (getPid(proc) == _arg1)
                break;
             
            qINSERT(proc, &aux);
        }
        
        while (qPOP(proc, &aux) == CMD_SUCCESS)
        {
            qINSERT(proc, Pqueue1);
        }
    }

    
    free(proc);

    // afiseaza coada + stivele pt pasul curent
    showOutput(g, stacks, *Pqueue1, nOfEvents, time);
}


int main(int argc, char * argv[])
{
    // nu ruleaza cu mai putin de 2 param. din linia de comanda
    if (argc != 3)
        return -1;
 

    FILE * f = fopen(argv[1], "r");
    FILE * g = fopen(argv[2], "w");

    int nOfEvents = 0;
    char command[MAX_CMD_SIZE+1];

    fscanf(f, "%d", &nOfEvents);

    int i = 0;
    TStack * stacks[nOfEvents]; // vectorul de stive (pt eventuri)

    for (i = 0; i < nOfEvents; i++)
        stacks[i] = NULL;

    TQueue * queue1 = NULL; // coada de prioritati
   
    int time = 0;
    
    while (fgets(command, MAX_CMD_SIZE, f) != NULL)
    {
        // parseaza fiecare linie si executa comanda aferenta
        execCommand(g, command, stacks, &queue1, nOfEvents, time);
        time++; // time = pasul curent
    }


    fclose(f);
    fclose(g);

    // elibereaza memoria folosita de coada
    disposeQueue(&queue1);
    
    // elibereaza memoria folosita de stive
    for (i = 0; i < nOfEvents; i++)
        disposeStack(&stacks[i]);    

    return 0;
}
