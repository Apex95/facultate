#ifndef _DB_H
#define _DB_H


typedef struct QueuedFileInfo
{
    char name[100];
    unsigned long size;
    char reqUpload;
} QueuedFileInfo;


void setCurrentUser(char*);
void setLogin();
char * getCurrentUser();
char isLoggedIn();
void setLogout();

char* getUploadChunk(char **, int *, char*);

void enqueueForUpload(char*);

void debugShowUploads();

#define ACCESS_PRIVATE 0
#define ACCESS_PUBLIC 1

#define MSG_SUCCESS "200"
#define MSG_INVALID_USER_PASS "-3"
#define MSG_TOO_MANY_FAILED_AUTHS "-8"
#define MSG_USER_NOT_EXISTS "-11"
#define MSG_FILE_NOT_EXISTS "-4"
#define MSG_ALREADY_LOGGED_IN "-2"
#define MSG_NOT_LOGGED_IN "-1"
#endif
