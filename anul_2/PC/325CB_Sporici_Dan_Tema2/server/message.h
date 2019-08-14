#ifndef _MESSAGE_H
#define _MESSAGE_H

#define BUFFER_SIZE 4096

enum MessageTypes
{
    TYPE_LOGIN = '1',
    TYPE_LOGOUT = '2',
    TYPE_GETUSERLIST = '3',
    TYPE_GETFILELIST = '4',
    TYPE_UPLOAD = '5',
    TYPE_SHARE = '6',
    TYPE_UNSHARE = '7',
    TYPE_DELETE = '8',
    TYPE_DOWNLOAD = '9'
};

typedef unsigned char MessageType;

typedef struct AuthMessage
{
    MessageType type;
    char user[100];
    char pass[100];
} AuthMessage;


// sa nu sara de 4096
#define USERS_PER_MSG 2

typedef struct UsersListMessage
{
    MessageType type;
    int nOfUsers;
    char lastMsg; // ultimul mesaj (cu ultimele elem. din lista)
    char users[USERS_PER_MSG][100];
} UsersListMessage;

typedef struct FileInfo
{
    char name[100];
    unsigned long size;
    char access;
} FileInfo;

#define FILES_PER_MSG 2

typedef struct FilesListMessage
{
    MessageType type;
    char owner[100];
    int nOfFiles;
    char lastMsg;
    FileInfo files[FILES_PER_MSG];
} FilesListMessage;

#define CHUNK_SIZE 3887
typedef struct FileUploadMessage
{
    MessageType type;
    char name[100];
    char owner[100];
    int size;
    char lastMsg;
    char content[CHUNK_SIZE];
} FileUploadMessage;

typedef struct FileOpMessage
{
    MessageType type;
    char name[100];
} FileOpMessage;

typedef struct Result
{
    int resCode;
    char resStr[4096]; // trebuie sa incapa o structura
} Result;

Result * handleMessage(int, char*);


#endif
