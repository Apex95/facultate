#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "db.h"
#include "message.h"

char * user = NULL;
char _isLoggedIn = 0;


// -- coada de upload
QueuedFileInfo * uploadsList = NULL;
int nOfUploads = 0;
int _maxUploads = 0;

int uploadIndex = 0;

FILE * f;

// seteaza utilizatorul cu care incearca sa 
// se autentifice (nu stiu daca ii si merge)
void setCurrentUser(char * usr)
{
    user = strdup(usr);
    _isLoggedIn = 0;
    // se incearca alta autentificare,
    // daca merge, se va seta ulterior
    // setLogin()
}

// seteaza daca s-a realizat autentificarea
void setLogin()
{
    _isLoggedIn = 1;
}

void setLogout()
{
    _isLoggedIn = 0;
    printf("Logged out\n");
}

char isLoggedIn()
{
    return _isLoggedIn;
}

char * getCurrentUser()
{
    if (_isLoggedIn)
        return user;
    else
        return NULL;
}

// adauga un fisier in lista pt un upload catre server
void enqueueForUpload(char * name)
{
    int i = 0;

    for (i = 0; i < _maxUploads; i++)
        if (!(uploadsList+i)->reqUpload)
        {
            memcpy((uploadsList+i)->name, name, strlen(name)+1);
            (uploadsList+i)->size = 0; // initial 0 bytes
            (uploadsList+i)->reqUpload = 1; 
            
            nOfUploads++;

            return; 
        }

    int _lastMaxUploads = _maxUploads;
    _maxUploads = 1 + 2 * _maxUploads;

    uploadsList = realloc(uploadsList, _maxUploads * sizeof(QueuedFileInfo));
    memset(uploadsList+_lastMaxUploads, 0, (_maxUploads-_lastMaxUploads) * sizeof(QueuedFileInfo));
    
    // adauga aici elem
    memcpy((uploadsList + _lastMaxUploads)->name, name, strlen(name)+1);
    (uploadsList + _lastMaxUploads)->size = 0;
    (uploadsList + _lastMaxUploads)->reqUpload = 1;
    
    nOfUploads++;
    
}

// preia urmatorul chunk din fisierul curent
char * getUploadChunk(char ** name, int * nOfBytes, char * isLastMsg)
{
    (*isLastMsg) = 0;

    if (_maxUploads == 0)
        return NULL;

    // daca fisierul curent nu necesita upload
    // caut altul...
    if (!(uploadsList+uploadIndex)->reqUpload)
    {
        int i = 0;
        for (i = 0; i < _maxUploads; i++)
            if ((uploadsList+i)->reqUpload)
            {
                uploadIndex = i;
                break;
            }

        // totul e uploadat?
        if (i == _maxUploads)
        {
            uploadIndex = 0;
            (*nOfBytes) = 0;
            return NULL;
        }

    }

    // ajunge aici => mai exista un fisier care n-a fost uploadat      
    char * buffer = malloc(CHUNK_SIZE);
    memset(buffer, 0, CHUNK_SIZE);

    // n-am mai uploadat nimic din fisierul asta? => deschid pt citire
    if ((uploadsList+uploadIndex)->size == 0)
        f = fopen((uploadsList+uploadIndex)->name, "rb"); 

    // intorc si numele fisierului
    (*name) = (uploadsList+uploadIndex)->name; 
            
    (*nOfBytes) = fread(buffer, 1, CHUNK_SIZE, f);

    // 0 octeti => nu merita sa mai intorc ceva
    if ((*nOfBytes) == 0)
    {
        (uploadsList+uploadIndex)->reqUpload = 0;
        fclose(f);
        return NULL;
    }

    (uploadsList+uploadIndex)->size += (*nOfBytes);

    unsigned char c = fgetc(f);
    // a citit mai putin decat voiam => e ultimul chunk
    if ((*nOfBytes) < CHUNK_SIZE || c == EOF)
    {
        (*isLastMsg) = 1;
        (uploadsList+uploadIndex)->reqUpload = 0;
        fclose(f);
    }
    else
        ungetc(c, f);

    return buffer;
    
}



// --- functii de debug ---

// afiseaza lista de uploads (fisierele pe care doreste sa le incarce clientul)
void debugShowUploads()
{
    int i = 0;

    printf("-------------------\n");
    printf("CrtUploads: %d | MaxUploads: %d\n", nOfUploads, _maxUploads);

    for (i = 0; i < _maxUploads; i++)
        printf("Name: %s | Size: %lu | ReqUpload: %d\n", (uploadsList+i)->name,
                                                         (uploadsList+i)->size,
                                                         (uploadsList+i)->reqUpload);
}
