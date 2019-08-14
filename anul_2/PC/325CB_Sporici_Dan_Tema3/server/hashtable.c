#include <stdlib.h>
#include <string.h>

#define MAX_URLS 3000

unsigned long getHash(char * url)
{
    unsigned long hash = 1;
    int c;

    while ((c = *url))
    {
        hash = ((hash << 5) + hash) + c;
        url++;
    }


    return hash % MAX_URLS;
}

char ** hashTable = NULL;

void createHashtable()
{
    hashTable = malloc(MAX_URLS * sizeof(char*));
    
    int i = 0;
    for (i = 0; i < MAX_URLS; i++)
        hashTable[i] = NULL;

}


int addToTable(char * url)
{
    // deja exista ceva alocat pe pozitia aia
    // posibil sa fie acelasi url (sau coliziune)
    
    if (hashTable == NULL)
        createHashtable();

    unsigned long hash = getHash(url);

    if (hashTable[hash] != NULL)
        return 0;

    
    hashTable[hash] = malloc(strlen(url)+1);
    memcpy(hashTable[hash], url, strlen(url)+1);

    return 1;
}
