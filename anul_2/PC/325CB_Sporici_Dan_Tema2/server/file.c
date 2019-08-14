#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "db.h"
#include "message.h"

// utilitar, creeaza un director
char makeDir(char * name)
{
    struct stat status;
    if (stat(name, &status) == -1)
    {
        mkdir(name, 0700);
        return 1;
    }
    else
        return 0;
}

// adauga toate fisierele gasite in director in filesTable
void loadFilesFromDir(char * name)
{
    DIR * dir = opendir(name);
    struct dirent *dirEnt;

    while ((dirEnt = readdir(dir)))
        if (strcmp(dirEnt->d_name, ".") != 0 && strcmp(dirEnt->d_name, "..") != 0)
            addFile(dirEnt->d_name, name, ACCESS_PRIVATE);
  
    free(dir); 
}

// scriere in fisier +verificari (pt upload)
char* writeToFile(int sock, FileUploadMessage m)
{
    if (!isLoggedIn(sock))
    {
        char * buffer = malloc(sizeof(FileUploadMessage));
        sprintf(buffer, "%d", MSG_REQUIRED_AUTH);
        return buffer;
    }


    char * user = getUser(sock);

    // verific sa nu fac "suprascrieri" (append pe fisiere deja complete)
    if (fileExists(m.name, user))
    {
        char * buffer = malloc(sizeof(FileUploadMessage));
        sprintf(buffer, "%d", MSG_FILE_ALREADY_EXISTS);
        return buffer;
    }
    

    char path[strlen(user) + strlen(m.name) + 2]; // + \0 + /
        
    memset(path, 0, sizeof(path));

    strcat(path, user);
    strcat(path, "/");
    strcat(path, m.name);

    FILE * f = fopen(path, "ab");
    fwrite(m.content, m.size, 1, f); 
    fclose(f);
 
    char msg[100];


    struct stat _st;
    stat(path, &_st);

    sprintf(msg, "Upload finished: %s - %lu bytes", m.name, _st.st_size); 

    char * buffer = malloc(sizeof(FileUploadMessage));
    memcpy(buffer, msg, strlen(msg)+1);


    if (m.lastMsg)
    {
        addFile(m.name, user, ACCESS_PRIVATE);
        return buffer;
    }
    else
    {
        free(buffer);
        return NULL;
    }
}


char * deleteFile(int sock, FileOpMessage m)
{
    if (!isLoggedIn(sock))
    {
        char * buffer = malloc(sizeof(FileUploadMessage));
        sprintf(buffer, "%d", MSG_REQUIRED_AUTH);
        return buffer;
    }


    char * user = getUser(sock);

    // verific sa nu fac "suprascrieri" (append pe fisiere deja complete)
    if (!fileExists(m.name, user))
    {
        char * buffer = malloc(sizeof(FileUploadMessage));
        sprintf(buffer, "%d", MSG_FILE_NOT_EXISTS);
        return buffer;
    }


    char path[strlen(user) + strlen(m.name) + 2]; // + \0 + /
        
    memset(path, 0, sizeof(path));

    strcat(path, user);
    strcat(path, "/");
    strcat(path, m.name);

    if (!removeFile(m.name, user))
    {
        char * buffer = malloc(sizeof(FileOpMessage));
        sprintf(buffer, "%d", MSG_FILE_IN_USE);
        return buffer;
    }
    unlink(path);
    //removeFile(m.name, user); 
    
    char * buffer = malloc(sizeof(FileOpMessage));
    sprintf(buffer, "%d", MSG_SUCCESS);
    return buffer;
}
