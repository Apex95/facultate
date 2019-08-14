#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "message.h"
#include "db.h"
#include "file.h"

// functie ce trateaza mesajele primite si apeleaza
// in functie de acestea alte functii specifice
// din db.c sau file.c
Result * handleMessage(int sock, char * str)
{
    // login...
    if (str[0] == TYPE_LOGIN)
    {
        AuthMessage m;

        memcpy(&m, str, sizeof(m));
        
        printf("User: %s | Pass: %s\n", m.user, m.pass);
        
        int resCode = authUser(sock, m.user, m.pass);
      
        Result * res = malloc(sizeof(Result));
        res->resCode = resCode;
        
        return res;         
    }

    if (str[0] == TYPE_LOGOUT)
    {
        int resCode = deauthUser(sock);
        Result * res = malloc(sizeof(Result));
        res->resCode = resCode;
         
        return res;
    }

    if (str[0] == TYPE_GETUSERLIST)
    {
        char * resStr = getUserList(sock);
       
        Result * res = malloc(sizeof(Result));
        res->resCode = MSG_IGNORE_CODE;
        memcpy(res->resStr, resStr, sizeof(UsersListMessage));
        
        free(resStr);        
        
        return res; 
    }

    if (str[0] == TYPE_GETFILELIST)
    {
        FilesListMessage m;
        memset(&m, 0, sizeof(m));
        memcpy(&m, str, sizeof(FilesListMessage));
        
        char * resStr = getFileList(sock, m.owner);

        Result * res = malloc(sizeof(Result));
        res->resCode = MSG_IGNORE_CODE;
        memcpy(res->resStr, resStr, sizeof(FilesListMessage));
        
        free(resStr);

        return res;

    }

    if (str[0] == TYPE_UPLOAD)
    {
        FileUploadMessage m;
        memset(&m, 0, sizeof(m));
        memcpy(&m, str, sizeof(FileUploadMessage));

        Result * res = malloc(sizeof(Result));
        
        char * resStr = writeToFile(sock, m); 

        if (resStr != NULL)
        {
            memcpy(res->resStr, resStr, sizeof(FileUploadMessage));
            res->resCode = 0;
            free(resStr);
        }
        else
            res->resCode = MSG_IGNORE_THIS;

        return res;
    }


    if (str[0] == TYPE_SHARE || str[0] == TYPE_UNSHARE)
    {
        FileOpMessage m;
        memset(&m, 0, sizeof(m));
        memcpy(&m, str, sizeof(FileOpMessage));

        Result * res = malloc(sizeof(Result));

        char * resStr;
        if (str[0] == TYPE_SHARE)
            resStr = setPublicAccess(sock, m.name);
        else
            resStr = setPrivateAccess(sock, m.name);

        memcpy(res->resStr, resStr, sizeof(FileOpMessage));
        free(resStr);
        return res;
    }

    if (str[0] == TYPE_DELETE)
    {
        FileOpMessage m;
        memset(&m, 0, sizeof(m));
        memcpy(&m, str, sizeof(m));

        Result * res = malloc(sizeof(Result));

        char * resStr;

        resStr = deleteFile(sock, m);

        memcpy(res->resStr, resStr, sizeof(FileOpMessage));
        free(resStr);
        return res;
    }

    if (str[0] == TYPE_DOWNLOAD)
    {
        FileUploadMessage m;
        memset(&m, 0, sizeof(m));
        memcpy(&m, str, sizeof(m));


        char * resStr = enqueueForUpload(sock, m.name, m.owner);
        
        Result * res = malloc(sizeof(Result));
        if (resStr != NULL)
        {
            memcpy(res->resStr, resStr, strlen(resStr)+1);
            free(resStr);
            res->resCode = 0;
        }
        else
            res->resCode = MSG_IGNORE_THIS;
        return res;
    }

    return 0; // ajunge aici daca primesc o comanda necunoscuta
}
