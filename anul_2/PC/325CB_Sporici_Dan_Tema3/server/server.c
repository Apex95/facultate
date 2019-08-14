#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<sys/types.h>


#include "server.h"
#include "hashtable.h"
#include "stack.h"

#define MAX_CLIENTS 10
#define BUFFER_SIZE 4096

void die(char * msg)
{
    printf("\n[ERR] %s\n\n", msg);
    exit(1);
}


void writeToFile(char hostname[], char path[], char data[], int dataSize)
{
    // aici primesc un path relativ de forma
    // /folder1/folder2/fisier

    int pathLen = strlen(path);
    int hostLen = strlen(hostname);
    
    // trebuie sa incapa adresa site-ului + path-ul + eventuale modificari
    // (ex: adaugare /index.html)
    char folderPath[pathLen + hostLen + 15];
    memset(folderPath, 0, sizeof(folderPath));

    strcat(folderPath, hostname);
    strcat(folderPath, path);

    int k = 0;
    pathLen = strlen(folderPath);

    while (folderPath[pathLen-1-k] != '.' &&
          folderPath[pathLen-1-k] != '/' &&
          k + 1 < pathLen)
          k++;


    // numele fisierului
    char fileName[100];
    memset(fileName, 0, sizeof(fileName));

    // daca ultimul din path e un fisier
    if (folderPath[pathLen-1-k] == '.')
    {
        // incerc sa scot numele fisierului de aici ca sa pot
        // face structura de foldere

        while (folderPath[pathLen-1-k] != '/' && k + 1 < pathLen)
            k++;

        

        if (folderPath[pathLen-1-k] == '/')
        {
            memcpy(fileName, folderPath + pathLen - k, k);
            memset(folderPath + pathLen - k, 0, k); 
        }

    }
    else
        // daca e un folder...
        if (folderPath[pathLen-1-k] == '/')
        {
            strcat(fileName, "index.html");
            
            if (folderPath[pathLen-1] != '/')
                strcat(folderPath, "/");


        }
        

    char _cmd[sizeof(folderPath) + 30];
    sprintf(_cmd, "%s%s", "mkdir -p ", folderPath); 
   
    //printf("%s\n", _cmd);
   
    system(_cmd);
    
    strcat(folderPath, fileName);

    

    if (access(folderPath, F_OK) == -1)
    {
        // fisierul nu exista, asta e primul chunk
        // deci va contine headerul http...care trebuie scos
        char * tempData = strstr(data, "\r\n\r\n") + 4; // sar si cele 4 caractere
        dataSize -= (tempData - data);
        data = tempData;

    }

    FILE * f = fopen(folderPath, "ab");

    if (f == NULL)
    {
        printf("Can't write: %s\n", folderPath);
        return;
    }
    fwrite(data, dataSize, 1, f); 
    fclose(f); 
}

int parseToCmd(char buffer[], Command * c)
{
    char * token = strtok(buffer, " ");

    printf("[%s]\n", token);

    if (strcmp(token, "status") == 0)
    {
        (*c).cmdCode = CMD_STATUS;
        return 0;
    }

    if (strcmp(token, "exit") == 0)
    {
        (*c).cmdCode = CMD_EXIT;
        return 0;
    }

    if (strcmp(token, "download") == 0)
    {
        (*c).cmdCode = CMD_DOWNLOAD;
        char * url = strtok(NULL, " ");

        memset((*c).url, 0, sizeof((*c).url));

        memcpy((*c).url, url, strlen(url)+1);

        return 0;
    }

    return -1;

}

char modesArray[3];

int main(int argc, char* argv[])
{
    int i = 0;
    int port = -1;

    // -- citire si configurare server
    memset(modesArray, 0, sizeof(modesArray));
    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-r") == 0)
            modesArray[RECURSIVE_OPTION] = 1;
        else
            if (strcmp(argv[i], "-e") == 0)
                modesArray[EVERYTHING_OPTION] = 1;
            else
                if (strcmp(argv[i], "-o") == 0)
                {
                    modesArray[LOG_OPTION] = 1;
                    i++;
                }
                else
                    if (strcmp(argv[i], "-p") == 0)
                    {
                        port = atoi(argv[i+1]);
                        i++;      
                    }
    }
    

    // -- pornire server
    struct sockaddr_in serverAddr, _clientAddr;
    Client clients[MAX_CLIENTS + 5];
    memset(clients, 0, sizeof(clients));
    int nOfClients = 0;

    int serverSock, clientSock;
    socklen_t  _clientSize = -1;
    int recvResult = -1;

    char buffer[BUFFER_SIZE];

    serverSock = socket(PF_INET, SOCK_STREAM, 0);

    if (serverSock < 0)
        die("Can't open socket");

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSock, (struct sockaddr *) &serverAddr, sizeof(struct sockaddr)) < 0)
        die("Can't bind.");


    printf("Waiting for clients...");
    listen(serverSock, MAX_CLIENTS); 

    // -- setez descriptorii
    int maxFd = -1;
    fd_set readFds; FD_ZERO(&readFds);
    fd_set tmpFds; FD_ZERO(&tmpFds);

    // aduag socketul pt listen
    FD_SET(serverSock, &readFds);

    FD_SET(STDIN_FILENO, &readFds);

    maxFd = serverSock;
    
    printf("Server sock: %d\n", serverSock); 


    struct timeval timer;
    timer.tv_sec = 3;
    timer.tv_usec = 0;

    // -- the great loop 
    while (1)
    {
        tmpFds = readFds;

        if (select(maxFd + 1, &tmpFds, NULL, NULL, &timer) == -1)
            die("Can't select");

        for (i = 0; i <= maxFd; i++)
        {
            // scot linkuri din stiva, le trimit inapoi la client
           if (i != serverSock && 
               i != STDIN_FILENO && clients[i].isConnected && !clients[i].isBusy &&
               nOfClients >= 5 &&
               (modesArray[RECURSIVE_OPTION] || modesArray[EVERYTHING_OPTION]))
           {
                char * newUrl = popUrl();

                // clientul e liber dar n-am ce comanda sa-i dau
                if (newUrl != NULL)
                {
                    Command cmd;
                    memset(&cmd, 0, sizeof(cmd));
                    cmd.cmdCode = CMD_DOWNLOAD; 

                    memcpy(cmd.url, newUrl, strlen(newUrl)+1);
                
                    memset(buffer, 0, sizeof(buffer));
                    memcpy(buffer, &cmd, sizeof(cmd));
                    
                    clients[i].isBusy = 1; 
                    send(i, buffer, sizeof(cmd), 0); 
                }
           }

            if (FD_ISSET(i, &tmpFds))
            {
                // ceva input de la utilizator
                if (i == 0)
                {
                    memset(buffer, 0, BUFFER_SIZE);
                    
                    fgets(buffer, BUFFER_SIZE-1, stdin);
                    buffer[strlen(buffer)-1] = 0;

                    Command currentCmd;
                    parseToCmd(buffer, &currentCmd);                    
             
             
                    printf("cmd: %d [%s]\n", currentCmd.cmdCode, currentCmd.url);
                    
                    
                    // -- comanda status
                    if (currentCmd.cmdCode == CMD_STATUS)
                    {
                        int j = 0;
                        for (j = 0; j <= maxFd; j++)
                        {
                            if (j == serverSock || j == STDIN_FILENO || !clients[j].isConnected)
                                continue;

                            printf("Client %d | IP: %s | PORT: %d\n", j,
                                                      inet_ntoa(clients[j].addr.sin_addr),
                                                      ntohs(clients[j].addr.sin_port)); 
                        }
                    }


                    // -- comanda download
                    if (currentCmd.cmdCode == CMD_DOWNLOAD)
                    {

                        if (addToTable(currentCmd.url))
                            pushUrl(currentCmd.url);

                        
                    }

                    memset(&currentCmd, 0, sizeof(currentCmd));                            
                    continue; 
                }



                // conexiune noua ?
                if (i == serverSock)
                {
                    _clientSize = sizeof(_clientAddr);
                    
                    clientSock = accept(serverSock, (struct sockaddr*) &_clientAddr, &_clientSize); 


                    if (clientSock < 0)
                        die("Can't accept new client");
                    else
                    {
                        FD_SET(clientSock, &readFds);

                        memcpy(&(clients[clientSock].addr), &_clientAddr, _clientSize);
                        clients[clientSock].isConnected = 1;

                        if (clientSock > maxFd)
                            maxFd = clientSock;
                    }

                    // initial n-are nimic de facut => not busy
                    clients[clientSock].isBusy = 0;

                    printf("Client connected - sock: %d\n", clientSock);
                    nOfClients++;
                }
                else
                {

                    // atlfel...mesaj de la un client
                    memset(buffer, 0, BUFFER_SIZE);
                    
                    // citesc primul octet
                    recvResult = recv(i, buffer, 1, 0);

                    // in functie de acesta, determin lungimea mesajului
                    // astfel incat sa nu citesc 2 mesaje in 1 (si sa pierd date)
                    if (buffer[0] == CMD_ADDLINK)
                        recvResult = recv(i, buffer+1, sizeof(Command)-1, 0);
                    else
                        if (buffer[0] == CMD_STOREDATA)
                            recvResult = recv(i, buffer+1, BUFFER_SIZE-1, 0);

                    if (recvResult == 0)
                    {
                        printf("Client %d disconnected\n", i);
                        clients[i].isConnected = 0;
                        nOfClients--;

                        close(i);
                        FD_CLR(i, &readFds);

                        continue;
                    }
                    else
                        if (recvResult < 0)
                            die("Can't recv");


                    // ajunge aici => clientul e activ, a trimis mesaj
  
                    // daca mesajul e fisier / pagina, il retin                   
                    if (buffer[0] == CMD_STOREDATA)
                    {
                        DataMsg msg;
                        memset(&msg, 0, sizeof(msg));

                        memcpy(&msg, buffer, recvResult);
                   
                        writeToFile(msg.hostname, msg.path, msg.data, msg.dataSize);
                    }
                    else
                    {
                        // daca mesajul e un link
                        if (buffer[0] == CMD_ADDLINK)
                        {

                            Command cmd;
                            memcpy(&cmd, buffer, recvResult);

                            // daca merge adaugat in hashtable
                            // inseamna ca n-a fost tratat linkul pana acum
                            if (addToTable(cmd.url))
                            {
                                // il adaug in stiva pt pagini
                                // ce vor trebui descarcate
                                
                                pushUrl(cmd.url);
                            }
                        }
                        else
                            if (buffer[0] == CMD_AVAILABLE)
                            {
                                clients[i].isBusy = 0;
                            }
                            else
                                printf("Sock %d: %s\n", i, buffer);
                    }
                }
            }
        }
    }

    return 0; 
}
