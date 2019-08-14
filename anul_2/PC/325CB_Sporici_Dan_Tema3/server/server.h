

enum serverOptions
{
    RECURSIVE_OPTION = 0,
    EVERYTHING_OPTION = 1,
    LOG_OPTION = 2
};

enum serverCommands
{
    CMD_STATUS = 0,
    CMD_DOWNLOAD = 1,
    CMD_EXIT = 2,


    CMD_STOREDATA = -1,
    CMD_ADDLINK = -2,
    CMD_AVAILABLE = -5
};

typedef struct Command
{
    char cmdCode;
    char url[300];
} Command;

typedef struct Client
{
    struct sockaddr_in addr;
    char isConnected;
    char isBusy;
} Client;

typedef struct DataMsg
{
    char cmdCode;
    char hostname[100];
    char path[300];
    char data[3691];
    int dataSize;

} DataMsg;
