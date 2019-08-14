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
    char _padding[BUFFER_SIZE - 201];
} AuthMessage;


#define USERS_PER_MSG 2
typedef struct UsersListMessage
{
    MessageType type;
    int nOfUsers;
    char lastMsg;
    char users[USERS_PER_MSG][100];
    char _padding[BUFFER_SIZE - 1 - sizeof(int) - 1 - USERS_PER_MSG * 100];
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
    char _padding[BUFFER_SIZE - 1 - 100 - sizeof(int) - 1 - sizeof(FileInfo) * FILES_PER_MSG];
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

// pt delete / share / unshare
typedef struct FileOpMessage
{
    MessageType type;
    char name[100];
    char _padding[BUFFER_SIZE - 100 - 1];
} FileOpMessage;


char * createMessage(char*, int*);
