#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include "db.h"
#include "message.h"
#include "file.h"

// conturi
AccountRow * accountsTable = NULL;

// fisiere
FileRow * filesTable = NULL;

// utilizatori conectati (asociez un sock cu un cont)
UserRow * usersTable = NULL;

int nOfAccounts = 0;
int nOfFiles = 0;
int nOfUsers = 0;


int _maxUsers = 0; // dimensiunea maxima a usersTable
int _maxFiles = 0; // dimensiunea maxima a filesTable

// incarca toate conturile gasite in fisierul de intrare
void loadAccounts(char * file)
{
    // user / pass
    FILE * f = fopen(file, "r");
    
    fscanf(f, "%d", &nOfAccounts);

    accountsTable = (AccountRow*)malloc(nOfAccounts * sizeof(AccountRow));
    
    int  i = 0;
    for (i = 0; i < nOfAccounts; i++)
    {
        fscanf(f, "%s %s", (accountsTable+i)->user, (accountsTable+i)->pass);
        
        // daca nu pot sa creez folderul => exista deja
        // sansa sa am fisiere prin el
        if (!makeDir((accountsTable+i)->user))
        {
            loadFilesFromDir((accountsTable+i)->user);
        }
        
    }   
    fclose(f);
}



// adaugarea unui fisier
void addFile(char * name, char * owner, char access)
{
    int i = 0;

    char _path[strlen(name) + strlen(owner)+2]; // owner/name\0
    memset(_path, 0, sizeof(_path));

    // construiesc path-ul
    strcat(_path, owner);
    strcat(_path, "/");
    strcat(_path, name);

    struct stat _stat;
    if (stat(_path, &_stat) != 0)
        return;

    // ^ functia 'addFile' e apelata doar in contextul
    // in care fisierul exista

    // caut prima zona in care pot adauga un fisier nou
    // eventual peste unul vechi (daca a fost sters recent)
    for (i = 0; i < _maxFiles; i++)
        if (!((filesTable+i)->exists))
        {
            memcpy((filesTable+i)->name, name, strlen(name)+1);
            memcpy((filesTable+i)->owner, owner, strlen(owner)+1);
            (filesTable+i)->access = access;
            (filesTable+i)->exists = 1;
            (filesTable+i)->size = _stat.st_size;

            nOfFiles++;

            return;
        }

    // ajunge aici => n-am gasit user deconectat
    // => trebuie mai mult loc
    int _lastMaxFiles = _maxFiles;
    _maxFiles = 1 + 2 * _maxFiles;

    filesTable = realloc(filesTable, _maxFiles * sizeof(FileRow));
    memset(filesTable+_lastMaxFiles, 0, (_maxFiles-_lastMaxFiles) * sizeof(FileRow)); 
    

    memcpy((filesTable + _lastMaxFiles)->name, name, strlen(name)+1);
    memcpy((filesTable + _lastMaxFiles)->owner, owner, strlen(owner)+1);
    (filesTable + _lastMaxFiles)->access = access;
    (filesTable + _lastMaxFiles)->exists = 1;
    (filesTable + _lastMaxFiles)->size = _stat.st_size;

    nOfFiles++;
}




// actualizeaza starea fisierelor (shared / private)
void loadShares(char * file)
{
    // user / file / access

    FILE * f = fopen(file, "r");
    int nOfShares = 0;

    fscanf(f, "%d\n", &nOfShares);

    char buffer[200];
    char * _owner = NULL;
    char * _name = NULL;

    int i = 0, j = 0;

    for (i = 0; i < nOfShares; i++)
    {
        fgets(buffer, sizeof(buffer), f);
        
        _owner = strtok(buffer, ":");

        _name = strtok(NULL, ":");
        _name[strlen(_name)-1] = '\0'; // sterg \n 


        for (j = 0; j < _maxFiles; j++)
            if (strcmp((filesTable+j)->owner, _owner) == 0 &&
                strcmp((filesTable+j)->name, _name) == 0 &&
                (filesTable+j)->exists)
            {
                (filesTable+j)->access = ACCESS_PUBLIC;
                break;
            }

        if (j == _maxFiles)
            printf("Nu exista fisierul [%s] al utilizatorului [%s]\n", _name, _owner);
    }

    debugShowFiles();
    fclose(f);
}


// adauga un utilizator nou in lista
void addUser(int sock)
{
    int i = 0;

    // caut prima zona in care pot adauga un nou user
    // eventual peste unul vechi (deconectat)
    for (i = 0; i < _maxUsers; i++)
        if (!((usersTable+i)->isConnected))
        {
            (usersTable+i)->sock = sock; // socketul 
            (usersTable+i)->isConnected = 1; // daca e considerat conectat sau nu (utilizat pt delete)
            (usersTable+i)->account = NULL; // contul asociat (NULL => neautentificat)
            (usersTable+i)->failedAuths = 0; // nr de autentificari esuate
            (usersTable+i)->uChunk = 0; // pt getuserlist, sa stiu ce users am listat si care au ramas
            (usersTable+i)->fChunk = 0; // tot un indice, dar pt getfilelist 
            (usersTable+i)->f = NULL; // stream pt download (pt fisierul curent)
            (usersTable+i)->downloadIndex = 0; // indicele pt download
            (usersTable+i)->_maxDownloads = 0; // nr maxim de fisiere in lista pt download
            (usersTable+i)->downloadsList = NULL; // lista de fisiere cerute de client pt download

            nOfUsers++;

            return;
        }

    // ajunge aici => n-am gasit user deconectat
    // => trebuie mai mult loc
    int _lastMaxUsers = _maxUsers;
    _maxUsers = 1 + 2 * _maxUsers;

    usersTable = realloc(usersTable, _maxUsers * sizeof(UserRow));
    memset(usersTable+_lastMaxUsers, 0, (_maxUsers-_lastMaxUsers) * sizeof(UserRow)); 
    

    (usersTable + _lastMaxUsers)->sock = sock;
    (usersTable + _lastMaxUsers)->isConnected = 1;
    (usersTable + _lastMaxUsers)->account = NULL;
    (usersTable + _lastMaxUsers)->failedAuths = 0;
    (usersTable + _lastMaxUsers)->uChunk = 0;
    (usersTable + _lastMaxUsers)->fChunk = 0;
    (usersTable+i)->f = NULL;
    (usersTable+i)->downloadIndex = 0;
    (usersTable+i)->_maxDownloads = 0;
    (usersTable+i)->downloadsList = NULL;


    nOfUsers++;
}

// sterge un utilizator din lista (cand s-a deconectat)
void removeUser(int sock)
{
    // nu e chiar remove, doar il marchez
    // ca deconectat iar locul e liber pt alt client

    int i = 0;

    for (i = 0; i < _maxUsers; i++)
    {
        if ((usersTable+i)->sock == sock)
        {
            (usersTable+i)->isConnected = 0;
            break;
        }
    }

    nOfUsers--;
}

int authUser(int sock, char* user, char * pass)
{
    AccountRow * crtRow = NULL;
    int i = 0;

    int msg = MSG_INVALID_USER_PASS;

    for (i = 0; i < nOfAccounts; i++)
        if (strcmp((accountsTable+i)->user, user) == 0)
        {
            // stiu ca user-ul exista => doar parola poate sa mai fie gresita
            msg = MSG_INVALID_USER_PASS;
            if (strcmp((accountsTable+i)->pass, pass) == 0) 
                crtRow = accountsTable+i;
        }

    for (i = 0; i < _maxUsers; i++)
        if ((usersTable+i)->sock == sock)
        {
            if (crtRow != NULL)
            {
                (usersTable+i)->account = crtRow;
                (usersTable+i)->failedAuths = 0;
                return MSG_SUCCESS;
            }
            else
            {
                (usersTable+i)->failedAuths++;

                // prea multe incercari esuate
                if ((usersTable+i)->failedAuths >= 3)
                    return MSG_TOO_MANY_FAILED_AUTHS;
            }
        }
        
    return msg;
}

// log out
int deauthUser(int sock)
{
    int i = 0;

    for (i = 0; i < _maxUsers; i++)
        if ((usersTable+i)->sock == sock)
        {
            // rup legatura dintre socket si contul existent
            (usersTable+i)->account = NULL;
            return MSG_SUCCESS; 
        }

    return MSG_SUCCESS;
}

// preluarea listei de fisiere
char * getFileList(int sock, char * owner)
{
    // pointer la indicele care retine pozitia
    // pe care trebuie s-o trimit din lista
    int * fChunk = getfChunkPtr(sock);

    if (!isLoggedIn(sock))
    {
        char * buffer = malloc(sizeof(FilesListMessage));
        sprintf(buffer, "%d", MSG_REQUIRED_AUTH);

        return buffer;
    }
    else
        if (!userExists(owner))
        {
            char * buffer = malloc(sizeof(FilesListMessage));
            sprintf(buffer, "%d", MSG_USER_NOT_EXISTS);

            return buffer;
        }
        else
        {
            int i = 0;

            FilesListMessage m;
            memset(&m, 0, sizeof(m));
        

            m.type = TYPE_GETFILELIST;
            m.lastMsg = 0;
            memcpy(m.owner, owner, strlen(owner)+1);

            int j = 0;

            for (i = 0, j = 0; j < FILES_PER_MSG && i + (*fChunk) < _maxFiles; i++)
            {
                //-- ar trebui incrementat i doar cand gasesc un fisier bun...
                // -- => iau alt indice
                if (strcmp((filesTable+i+(*fChunk))->owner, owner) == 0 &&
                    (filesTable+i+(*fChunk))->exists)
                {
                    // copiez numele fisierului
                    memcpy(m.files[j].name, (filesTable+i+(*fChunk))->name, strlen((filesTable+i+(*fChunk))->name)+1); 
                    m.files[j].size =  (filesTable+i+(*fChunk))->size;
                    m.files[j].access = (filesTable+i+(*fChunk))->access;

                    j++;
                }
            }
            m.nOfFiles = j;

            (*fChunk) += i; // actualizez indicele curent

            if ((*fChunk) >= _maxFiles)
            {
                m.lastMsg = 1;
                (*fChunk) = 0;
            }


            char * buffer = malloc(sizeof(m));
            memcpy(buffer, &m, sizeof(m));

            return buffer;
        }

}

// verifica daca utilizatorul curent are cont
char userExists(char * name)
{
    int i = 0;
    
    for (i = 0; i < nOfAccounts; i++)
        if (strcmp((accountsTable+i)->user, name) == 0)
            return 1;
    
    return 0;
}

// intoarce pointer la indicele listei de fisiere
int * getfChunkPtr(int sock)
{
    int i = 0;

    for (i = 0; i < _maxUsers; i++)
        if ((usersTable+i)->sock == sock)
            return &((usersTable+i)->fChunk);

    return NULL;
}

// intoarce lista de utilizatori
char * getUserList(int sock)
{
    int * uChunk = getuChunkPtr(sock);

    if (!isLoggedIn(sock))
    {
        char * buffer = malloc(sizeof(UsersListMessage));
        sprintf(buffer, "%d", MSG_REQUIRED_AUTH);

        return buffer;
    }
    else
    { 
        int i = 0;

        UsersListMessage m;
        memset(&m, 0, sizeof(m));
        
        m.type = TYPE_GETUSERLIST;
        m.lastMsg = 0;

        //memset(m.users, 0, USERS_PER_MSG);

        for (i = 0; i < USERS_PER_MSG && i + (*uChunk) < nOfAccounts; i++)
        {
            memcpy(m.users[i], (accountsTable+i+(*uChunk))->user, strlen((accountsTable+i+(*uChunk))->user) +1);
        }
        m.nOfUsers = i;

        (*uChunk) += i;

        if ((*uChunk) >= nOfAccounts)
        {
            m.lastMsg = 1;
            (*uChunk) = 0;
        }


        char * buffer = malloc(sizeof(m));
        memcpy(buffer, &m, sizeof(m));


        return buffer;
    }
}

// pointer la indicele ce contine zona (din lista) din care sa trimita
// urmatorii utilizatori 
int * getuChunkPtr(int sock)
{
    int i = 0;

    for (i = 0; i < _maxUsers; i++)
        if ((usersTable+i)->sock == sock)
            return &((usersTable+i)->uChunk);

    return NULL; // tehnic vorbind n-ar avea cum sa ajunga aici
}

// daca socketului respectiv ii corespunde un cont
char isLoggedIn(int sock)
{
    int i = 0;
    for (i = 0; i < _maxUsers; i++)
        if ((usersTable+i)->sock == sock && (usersTable+i)->account != NULL)
            return 1;

    return 0;
}

// cauta numele utilizatorului stiind sock-ul
char * getUser(int sock)
{
    int i = 0;

    for (i = 0; i < nOfUsers; i++)
        if ((usersTable+i)->sock == sock)
            return (usersTable+i)->account->user;

    return NULL; 
}

// verific daca fisierul exista in lista
char fileExists(char* name, char * owner)
{
    int i = 0;

    for (i = 0; i < _maxFiles; i++)
        if (strcmp((filesTable+i)->name, name) == 0 && 
            strcmp((filesTable+i)->owner, owner) == 0 &&
            (filesTable+i)->exists)
            return 1;

    return 0;
}

// share
char* setPublicAccess(int sock, char * name)
{
    if (!isLoggedIn(sock))
    {
        char * buffer = malloc(sizeof(FileOpMessage));
        sprintf(buffer, "%d", MSG_REQUIRED_AUTH);
        return buffer;
    }

    char * owner = getUser(sock);

    int i = 0;

    for (i = 0; i < _maxFiles; i++)
        if (strcmp((filesTable+i)->name, name) == 0 &&
            strcmp((filesTable+i)->owner, owner) == 0 &&
            (filesTable+i)->exists)
        {
            char * buffer = malloc(sizeof(FileOpMessage));
            if ((filesTable+i)->access == ACCESS_PUBLIC)
            { 
                sprintf(buffer, "%d", MSG_FILE_ALREADY_PUBLIC);
                return buffer;
            }

            (filesTable+i)->access = ACCESS_PUBLIC;
            sprintf(buffer, "%d", MSG_SUCCESS);
            return buffer;
        }

    char * buffer = malloc(sizeof(FileOpMessage));
    sprintf(buffer, "%d", MSG_FILE_NOT_EXISTS);
    return buffer;
}

// unshare
char* setPrivateAccess(int sock, char * name)
{
    if (!isLoggedIn(sock))
    {
        char * buffer = malloc(sizeof(FileOpMessage));
        sprintf(buffer, "%d", MSG_REQUIRED_AUTH);
        return buffer;
    }

    char * owner = getUser(sock);

    int i = 0;

    for (i = 0; i < _maxFiles; i++)
        if (strcmp((filesTable+i)->name, name) == 0 &&
            strcmp((filesTable+i)->owner, owner) == 0 &&
            (filesTable+i)->exists)
        {
            char * buffer = malloc(sizeof(FileOpMessage));
            if ((filesTable+i)->access == ACCESS_PRIVATE)
            { 
                sprintf(buffer, "%d", MSG_FILE_ALREADY_PRIVATE);
                return buffer;
            }

            sprintf(buffer, "%d", MSG_SUCCESS);
            (filesTable+i)->access = ACCESS_PRIVATE;
            return buffer;
        }

    char * buffer = malloc(sizeof(FileOpMessage));
    sprintf(buffer, "%d", MSG_FILE_NOT_EXISTS);
    return buffer;
}

// verific daca am acces
// (fisierul e al meu sau e partajat)
char haveAccess(int sock, char * name, char * owner)
{
    int i = 0;
    char * downloadUser = getUser(sock);
    
    for (i = 0; i < _maxFiles; i++)
        if (strcmp(downloadUser, owner) == 0) // user care descarca propriul fisier
        {
            if (strcmp((filesTable+i)->name, name) == 0)
                return 1;
        }
        else // fisierul apartine altui utilizator
            if ((filesTable+i)->access == ACCESS_PUBLIC)
                if (strcmp((filesTable+i)->name, name) == 0 && strcmp((filesTable+i)->owner, owner) == 0)
                {
                    printf("MERGE\n");
                    return 1;
                }

    return 0;

}

// adauga in lista pt download (un fel de upload facut catre client)
char* enqueueForUpload(int sock, char * name, char * owner)
{
    if (!isLoggedIn(sock))
    {
        char * buffer = malloc(sizeof(FileUploadMessage));
        sprintf(buffer, "%d", MSG_REQUIRED_AUTH); 
        return buffer;
    }
    if (!fileExists(name, owner))
    {
        char * buffer = malloc(sizeof(FileUploadMessage));
        sprintf(buffer, "%d", MSG_FILE_NOT_EXISTS);
        return buffer; 
    }

    if (!haveAccess(sock, name, owner))
    {
        char * buffer = malloc(sizeof(FileUploadMessage));
        sprintf(buffer, "%d", MSG_UNAUTHORIZED_DOWNLOAD);
        return buffer;
    }

    // caut utilizatorul cu sock-ul dat
    int i = 0;
    for (i = 0; i < _maxUsers; i++)
        if ((usersTable+i)->sock == sock && (usersTable+i)->isConnected)
            break;

    QueuedFileInfo ** downloadsList = &((usersTable+i)->downloadsList);
    int * _maxDownloads = &((usersTable+i)->_maxDownloads);

    char path[strlen(name) + strlen(owner)+2];
    memset(path, 0, sizeof(path));
    strcat(path, owner);
    strcat(path, "/");
    strcat(path, name);


    i = 0;

    for (i = 0; i < *_maxDownloads; i++)
        if (!(*downloadsList + i)->reqDownload)
        {
            memcpy((*downloadsList + i)->name, name, strlen(name)+1);
            (*downloadsList+i)->size = 0; // initial 0 bytes
            (*downloadsList+i)->reqDownload = 1; 
            memcpy((*downloadsList+i)->path, path, strlen(path)+1); 

            return NULL; 
        }

    int _lastMaxDownloads = *_maxDownloads;
    *_maxDownloads = 1 + 2 * (*_maxDownloads);

    *downloadsList = realloc(*downloadsList, *_maxDownloads * sizeof(QueuedFileInfo));
    memset(*downloadsList + _lastMaxDownloads, 0, (*_maxDownloads - _lastMaxDownloads) * sizeof(QueuedFileInfo));
    
    // adauga aici elem
    memcpy((*downloadsList + _lastMaxDownloads)->name, name, strlen(name)+1);
    (*downloadsList + _lastMaxDownloads)->size = 0;
    (*downloadsList + _lastMaxDownloads)->reqDownload = 1;
    memcpy((*downloadsList+i)->path, path, strlen(path)+1); 

    return NULL;
}


// intoarce continutul ramas necitit din fisier
char * getUploadChunk(int sock, char ** name, int * nOfBytes, char * isLastMsg)
{
    // caut utilizatorul cu sock-ul dat
    int i = 0;
    for (i = 0; i < _maxUsers; i++)
        if ((usersTable+i)->sock == sock && (usersTable+i)->isConnected)
            break;
    
    if (i == _maxUsers)
        return NULL;

    QueuedFileInfo ** downloadsList = &((usersTable+i)->downloadsList);
    int * downloadIndex = &((usersTable+i)->downloadIndex);
    int * _maxDownloads = &((usersTable+i)->_maxDownloads);
    FILE ** f = &((usersTable+i)->f);

    if (*_maxDownloads == 0)
        return NULL;
    

    (*isLastMsg) = 0;

    if (_maxDownloads == 0)
        return NULL;

    // daca fisierul curent nu necesita download
    // caut altul...
    if (!((*downloadsList + *downloadIndex)->reqDownload))
    {
        int i = 0;
        for (i = 0; i < *_maxDownloads; i++)
            if ((*downloadsList+i)->reqDownload)
            {
                *downloadIndex = i;
                break;
            }

        // totul e downloadat
        if (i == *_maxDownloads)
        {
            *downloadIndex = 0;
            (*nOfBytes) = 0;
            return NULL;
        }

    }

    // ajunge aici => mai exista un fisier care n-a fost downloadat      
    char * buffer = malloc(CHUNK_SIZE);
    memset(buffer, 0, CHUNK_SIZE);

    // n-am mai downloadat nimic din fisierul asta? => deschid pt citire
    if ((*downloadsList + *downloadIndex)->size == 0)
        *f = fopen((*downloadsList + *downloadIndex)->path, "rb"); 

    // intorc si numele fisierului
    (*name) = (*downloadsList + *downloadIndex)->name; 
            
    (*nOfBytes) = fread(buffer, 1, CHUNK_SIZE, *f);

    // 0 octeti => nu merita sa mai intorc ceva
    if ((*nOfBytes) == 0)
    {
        (*downloadsList + *downloadIndex)->reqDownload = 0;
        fclose(*f);
        return NULL;
    }

    (*downloadsList + *downloadIndex)->size += (*nOfBytes);

    unsigned char c = fgetc(*f);
    // a citit mai putin decat voiam => e ultimul chunk
    if ((*nOfBytes) < CHUNK_SIZE || c == EOF)
    {
        (*isLastMsg) = 1;
        (*downloadsList + *downloadIndex)->reqDownload = 0;
        fclose(*f);
    }
    else
        ungetc(c, *f);

    return buffer;
    
}


// functia delete
char removeFile(char * name, char * owner)
{
    char path[strlen(name) + strlen(owner)+2];
    memset(path, 0, sizeof(path));
    strcat(path, owner);
    strcat(path, "/");
    strcat(path, name);

    int i = 0, j = 0;
    // verific daca fisierul nu e cerut pt download de un user
    for (i = 0; i < _maxUsers; i++)
        for (j = 0; j < (usersTable+i)->_maxDownloads; j++)
            if (strcmp((usersTable+i)->downloadsList[j].path, path) == 0)
                return 0; // fara remove 

    for (i = 0; i < _maxFiles; i++)
        if (strcmp((filesTable+i)->name, name) == 0 &&
            strcmp((filesTable+i)->owner, owner) == 0 &&
            (filesTable+i)->exists)
            (filesTable+i)->exists = 0;

    return 1;
}




//---functii pt debug---

// afiseaza lista de utilizatori 
void debugShowUsers()
{
    int i = 0;

    printf("Crt: %d\nMax: %d\n", nOfUsers, _maxUsers);

    for (i = 0; i < _maxUsers; i++)
        printf("Sock: %d | Account: %p | uChunk: %d\n", (usersTable+i)->sock,
                                                            (usersTable+i)->account,
                                                            (usersTable+i)->uChunk);
}

// lista de fisiere
void debugShowFiles()
{
    int i = 0;

    for (i = 0; i < _maxFiles; i++)
        printf("File: %s | Owner: %s | Size: %lu | Access: %d\n", (filesTable+i)->name,
                                                      (filesTable+i)->owner,
                                                      (filesTable+i)->size,
                                                      (filesTable+i)->access);
}

// lista de conturi (din fisier)
void debugShowAccounts()
{

    int i = 0;

    for (i = 0; i < nOfAccounts; i++)
        printf("User: %s | Pass: %s\n", (accountsTable+i)->user,
                                      (accountsTable+i)->pass);
}


