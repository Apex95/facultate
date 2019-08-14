#include <stdio.h>
#include <stdlib.h>


#define IS_ALIVE(c) (c == 'X' ? 1 : 0)
#define IS_DEAD(c) (c == '.' ? 1 : 0)

#define SET_DEAD(c) (c = '.')

#define SET_ALIVE(c) (c = 'X')

void copyData(char ** data, char ** newData, long rows, long cols)
{
    long i = 0, j = 0;

    for (i = 1; i < rows+1; i++)
        for (j = 1; j < cols+1; j++)
        {
            data[i][j] = newData[i][j];

            // bordare prima coloana
            if (j == cols)
                data[i][0] = data[i][j];

            // bordare ultima coloana
            if (j == 1)
                data[i][cols+1] = data[i][j];

            // bordare prima linie
            if (i == rows)
                data[0][j] = data[i][j];

            // bordare ultima linie
            if (i == 1)
                data[rows+1][j] = data[i][j];

            // coltul stanga-sus
            if (i == rows && j == cols)
                data[0][0] = data[i][j];

            // coltul stanga-jos
            if (i == 1 && j == cols)
                data[rows+1][0] = data[i][j];
                
            // coltul dreapta-sus
            if (i == rows && j == 1)
                data[0][cols+1] = data[i][j];

            // coltul drepta-jos
            if (i == 1 && j == 1)
                data[rows+1][cols+1] = data[i][j];
        }
}

void runEpoch(char ** data, char ** newData, long rows, long cols)
{
   long i, j, adjacentCells;
   
   for (i = 1; i < rows+1; i++)
       for (j = 1; j < cols+1; j++)
       {
           adjacentCells = IS_ALIVE(data[i-1][j-1]) + IS_ALIVE(data[i-1][j])   +
                           IS_ALIVE(data[i-1][j+1]) + IS_ALIVE(data[i][j-1])   +
                           IS_ALIVE(data[i][j+1])   + IS_ALIVE(data[i+1][j-1]) +
                           IS_ALIVE(data[i+1][j])   + IS_ALIVE(data[i+1][j+1]);

           
           
           // mai putin de 2 vecini => dead
           if (IS_ALIVE(data[i][j]) && adjacentCells < 2)
               SET_DEAD(newData[i][j]); 
           else
           // e in viata, are 2-3 vecini => mai traieste
           if (IS_ALIVE(data[i][j]) && (adjacentCells == 2 || adjacentCells == 3))
               SET_ALIVE(newData[i][j]); 
           else
           // are peste 3 vecini => dead
           if (IS_ALIVE(data[i][j]) && adjacentCells > 3)
               SET_DEAD(newData[i][j]);
           else
           // dead, cu fix 3 vecini => alive
           if (IS_DEAD(data[i][j]) && adjacentCells == 3)
               SET_ALIVE(newData[i][j]);
           else
               newData[i][j] = data[i][j];
                
                
       }

}


long main(long argc, char * argv[])
{
    // <input> <iteratii> <output>
    if (argc != 4)
        return EXIT_FAILURE;

    char * inputFile = argv[1];
    char * outputFile = argv[3];
    long nOfIterations = strtol(argv[2], NULL, 0);

    FILE * inputStream = fopen(inputFile, "r");

    long rows = 0, cols = 0;
    fscanf(inputStream, "%ld %ld\n", &rows, &cols);


    // -- alocare matrice
    char ** data = malloc(sizeof(char *) * (rows+2));
    char ** newData = malloc(sizeof(char *) * (rows+2));

    if (data == NULL || newData == NULL)
        return EXIT_FAILURE;
    
    long i = 0, j = 0;

    for (i = 0; i < rows+2; i++)
    {
        data[i] = malloc(sizeof(char) * (cols+2));
        newData[i] = malloc(sizeof(char) * (cols+2));

        if (data[i] == NULL || newData[i] == NULL)
            return EXIT_FAILURE;
    }
    // -- sfarsit alocare


    // -- citire matrice
    for (i = 1; i < rows+1; i++)
        for (j = 1; j < cols+1; j++)
        {
            fscanf(inputStream, "%c ", &(data[i][j]));

            // bordare prima coloana
            if (j == cols)
                data[i][0] = data[i][j];

            // bordare ultima coloana
            if (j == 1)
                data[i][cols+1] = data[i][j];

            // bordare prima linie
            if (i == rows)
                data[0][j] = data[i][j];

            // bordare ultima linie
            if (i == 1)
                data[rows+1][j] = data[i][j];

            // coltul stanga-sus
            if (i == rows && j == cols)
                data[0][0] = data[i][j];

            // coltul stanga-jos
            if (i == 1 && j == cols)
                data[rows+1][0] = data[i][j];
                
            // coltul dreapta-sus
            if (i == rows && j == 1)
                data[0][cols+1] = data[i][j];

            // coltul drepta-jos
            if (i == 1 && j == 1)
                data[rows+1][cols+1] = data[i][j];
        }
    
    fclose(inputStream); 
    // -- sfarsit citire

    for (i = 0; i < nOfIterations; i++)
    {
        runEpoch(data, newData, rows, cols);
        copyData(data, newData, rows, cols);
    }

    FILE * outputStream = fopen(outputFile, "w");

    free(data[0]); free(newData[0]);
    free(data[rows+1]); free(newData[rows+1]);

    for (i = 1; i < rows+1; i++)
    {
        for (j = 1; j < cols+1; j++)
        {
            fprintf(outputStream, "%c ", data[i][j]);
        }

        free(data[i]);
        free(newData[i]);

        fprintf(outputStream, "\n");
    }
    fclose(outputStream);

    free(data);
    free(newData);

    return EXIT_SUCCESS;

}


