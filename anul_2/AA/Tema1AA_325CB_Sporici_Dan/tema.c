#include<stdio.h>
#include<string.h>

typedef struct Celula
{
    int stare_urmatoare;
    char caracter_scris;
    char deplasare;
} Celula;



/* trece un caracter din banda prin matricea de tranzitie */

void interpreteaza_input(Celula mat[][3], char * w, int * stare, int * i)
{
    int c = -1;
    switch(w[*i])
    {
        case '0':
            c = 0;
            break;

        case '1':
            c = 1;
            break;

        case '#':
            c = 2;
            break;
    }



    w[*i] = mat[*stare][c].caracter_scris;
    if (mat[*stare][c].deplasare == 'R')
        (*i)++;

    if (mat[*stare][c].deplasare == 'L')
        (*i)--;

    *stare = mat[*stare][c].stare_urmatoare;
}



int main()
{
    char w[200]; // banda

    FILE * f = fopen("date.in", "r");
    fscanf(f, "%s\n", w);

    int s = 0;
    int sf = 0;
    
    fscanf(f, "%d %d\n", &s, &sf);
    


    int i = 0; int j = 0;
    
    // matricea de tranzitie
    Celula mat[s-sf][3];

    for (i = 0; i < s-sf; i++)
    {
        for (j = 0; j < 3; j++)
        {
            fscanf(f, "%d", &mat[i][j].stare_urmatoare);
        
            // stare nedefinita ?
            if (mat[i][j].stare_urmatoare != -1)
                fscanf(f, " %c %c", &mat[i][j].caracter_scris,
                                    &mat[i][j].deplasare);
        
        }    
    }

    fclose(f);
    
    // i = indice in banda / incepe de la al 2-lea caracter
    i = 1;
    int dim_lista = strlen(w);
    int c_s = 0; // starea curenta

    
    f = fopen("date.out", "w");

    while (c_s < s-sf && i < dim_lista && i >= 0)
    {
       interpreteaza_input(mat, w, &c_s, &i);

       // am nimerit starea nedefinita ?
       if (c_s == -1)
       {
           fprintf(f, "Eroare!");
           fclose(f);  
           return -1;
       }
    }
    
    
    for (i = 0; i < dim_lista; i++)
        fprintf(f, "%c", w[i]);
        
    fclose(f);

    return 0;
}
