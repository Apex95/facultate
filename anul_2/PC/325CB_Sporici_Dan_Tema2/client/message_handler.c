#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "message.h"
#include "db.h"

char* createMessage(char * str, int * newSize)
{
    // toate mesajele sunt considerate bune

    // salvez comanda originala - pt ca strtok nu
    // merge bine pt fisiere cu spatii in nume...
    char * originalStr = strdup(str);
    char * _token = strtok(str, " ");

    if (_token == NULL)
    {
        (*newSize) = -1;
        return NULL;
    }

    // -- daca e request de log in
    if (strcmp(_token, "login") == 0)
    {
        // e deja logat si mai incearca o autentificare...
        // nici n-are rost sa mai compun mesajul
        if (isLoggedIn())
        {
            printf("%s\n", MSG_ALREADY_LOGGED_IN);
            return NULL;
        }

        AuthMessage m;
        m.type = TYPE_LOGIN;
        
        _token = strtok(NULL, " ");
        memset(m.user, 0, sizeof(m.user));
        memcpy(m.user, _token, strlen(_token));

        _token = strtok(NULL, " ");
        memset(m.pass, 0, sizeof(m.pass));
        memcpy(m.pass, _token, strlen(_token));

        char * buffer = malloc(sizeof(m));

        memcpy(buffer, &m, sizeof(m));

        (*newSize) = sizeof(m);

        // setez utilizatorul curent
        setCurrentUser(m.user); 

        return buffer;
    }


    // -- daca e request de log out
    if (strcmp(_token, "logout") == 0)
    {
        if (!isLoggedIn())
        {
            printf("%s\n", MSG_NOT_LOGGED_IN);
            return NULL;
        }

        AuthMessage m;
        m.type = TYPE_LOGOUT;

        // celelalte campuri nu conteaza
        
        char * buffer = malloc(sizeof(m));
        memcpy(buffer, &m, sizeof(m));

        (*newSize) = sizeof(m);

        //setLogout();

        return buffer;
    }


    if (strcmp(_token, "getuserlist") == 0)
    {
        char * buffer = malloc(sizeof(UsersListMessage));
        buffer[0] = TYPE_GETUSERLIST;

        (*newSize) = sizeof(UsersListMessage);

        return buffer;
        
    }


    if (strcmp(_token, "getfilelist") == 0)
    {
        FilesListMessage m;
        memset(&m, 0, sizeof(m));

        m.type = TYPE_GETFILELIST;

        _token = strtok(NULL, " ");
        memcpy(m.owner, _token, strlen(_token)+1); 

        char * buffer = malloc(sizeof(m));
        memcpy(buffer, &m, sizeof(m));
        
        (*newSize) = sizeof(m);

        return buffer;
    }

    if (strcmp(_token, "upload") == 0)
    {
       char * name = strchr(originalStr, ' ') + 1;
       

       // daca fisierul nu exista...
       if (access(name, F_OK) == -1)
       {
           printf("%s\n", MSG_FILE_NOT_EXISTS);
           return NULL;
       }

       enqueueForUpload(name);
       
       return NULL;  
    }


    if (strcmp(_token, "share") == 0)
    {
        char * name = strchr(originalStr, ' ') + 1; // +1 sa scot spatiul

        //printf("[%s]\n", name);

        FileOpMessage m;
        memset(&m, 0, sizeof(m));

        memcpy(m.name, name, strlen(name)+1);
        m.type = TYPE_SHARE;

        char * buffer = malloc(sizeof(m));
        memcpy(buffer, &m, sizeof(m));

        (*newSize) = sizeof(FileOpMessage);

        return buffer;

    }

    if (strcmp(_token, "unshare") == 0)
    {
        char * name = strchr(originalStr, ' ') + 1;

        //printf("[%s]\n", name);

        FileOpMessage m;
        memset(&m, 0, sizeof(m));

        memcpy(m.name, name, strlen(name)+1);
        m.type = TYPE_UNSHARE;

        char * buffer = malloc(sizeof(m));
        memcpy(buffer, &m, sizeof(m));

        (*newSize) = sizeof(FileOpMessage);

        return buffer;

    }

    if (strcmp(_token, "delete") == 0)
    {
        char * name = strchr(originalStr, ' ') + 1;
        
        FileOpMessage m;
        memset(&m, 0, sizeof(m));

        memcpy(m.name, name, strlen(name)+1);
        m.type = TYPE_DELETE;

        char * buffer = malloc(sizeof(m));
        memcpy(buffer, &m, sizeof(m));

        (*newSize) = sizeof(FileOpMessage);

        return buffer;
    }

    if (strcmp(_token, "download") == 0)
    {
        originalStr += sizeof("download");

        char * name = strchr(originalStr, ' ');
        name[0] = 0;
        name++;

        if (strcmp(originalStr, "@") == 0)
            originalStr = getCurrentUser();
        //printf("[%s][%s]\n", originalStr, name);

        FileUploadMessage m;
        memset(&m, 0, sizeof(m));

        memcpy(m.owner, originalStr, strlen(originalStr)+1);
        memcpy(m.name, name, strlen(name)+1);
        m.type = TYPE_DOWNLOAD;

        char * buffer = malloc(sizeof(m));
        memcpy(buffer, &m, sizeof(m));

        (*newSize) = sizeof(FileUploadMessage);
        return buffer;
    }


    (*newSize) = -1;
    return NULL;

}
