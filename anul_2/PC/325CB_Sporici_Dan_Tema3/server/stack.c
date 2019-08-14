#include<stdlib.h>
#include<string.h>

#define MAX_URLS 3000

char ** stack = NULL;

int stackIndex = 0;

void createStack()
{
    stack = malloc(MAX_URLS * sizeof(char*));

    stackIndex = 0;

    int i = 0;
    for (i = 0; i < MAX_URLS; i++)
        stack[i] = NULL;
}

void pushUrl(char * url)
{
    if (stack == NULL)
        createStack();

    // cumva s-a umplut stiva
    if (stackIndex+1 >= MAX_URLS)
        return; // pierd niste url-uri...sau maresc stiva
        
    stackIndex++;
    stack[stackIndex] = malloc(strlen(url)+1);
    memcpy(stack[stackIndex], url, strlen(url)+1);
     
}


char * popUrl()
{
    if (stackIndex < 0 || stack == NULL)
        return NULL;
    return stack[stackIndex--];
}
