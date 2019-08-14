#ifndef _DB_H
#define _DB_H
typedef struct AccountRow
{
    char user[100];
    char pass[100];
} AccountRow;

typedef struct FileRow
{
    char name[100];
    char owner[100];
    char access;
    char exists;
    unsigned long size;
} FileRow;

typedef struct QueuedFileInfo
{
    char name[100];
    char path[201];
    unsigned long size;
    char reqDownload;
} QueuedFileInfo;

typedef struct UserRow
{
    int sock;
    char isConnected;
    int failedAuths;
    int uChunk;
    int fChunk;
    FILE * f;
    int downloadIndex; // indicele pt ce fisier descarca utilizatorul 
    int _maxDownloads;
    AccountRow * account;
    QueuedFileInfo * downloadsList;
} UserRow;





void loadAccounts(char*);
void loadShares(char*);
void addUser(int);
void removeUser(int);
int authUser(int, char*, char*);
int deauthUser(int);
char isLoggedIn(int);
char * getUserList(int);
void addFile(char*, char*, char);
char * getFileList(int, char*);
char userExists(char*);
char* getUser(int);
char fileExists(char*, char*);
char* setPublicAccess(int, char*);
char* setPrivateAccess(int, char*);
char removeFile(char*, char*);
char* getUploadChunk(int, char **, int *, char *);
char* enqueueForUpload(int, char*, char*);

int * getuChunkPtr(int);
int * getfChunkPtr(int);

void debugShowUsers();
void debugShowAccounts();
void debugShowFiles();


#define ACCESS_PRIVATE 0
#define ACCESS_PUBLIC 1


#define MSG_SUCCESS 200
#define MSG_INVALID_USER_PASS -3
#define MSG_TOO_MANY_FAILED_AUTHS -8
#define MSG_USER_NOT_EXISTS -11
#define MSG_REQUIRED_AUTH -1
#define MSG_FILE_ALREADY_EXISTS -9
#define MSG_FILE_NOT_EXISTS -4
#define MSG_FILE_ALREADY_PUBLIC -6
#define MSG_FILE_ALREADY_PRIVATE -7
#define MSG_UNAUTHORIZED_DOWNLOAD -5
#define MSG_FILE_IN_USE -10

#define MSG_IGNORE_CODE -99999
#define MSG_IGNORE_THIS -99998

#endif
