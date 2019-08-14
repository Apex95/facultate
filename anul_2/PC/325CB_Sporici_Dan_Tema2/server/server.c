
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "db.h"
#include "message.h"

#define MAX_CLIENTS 5



void die(char* errmsg)
{
    printf("%s", errmsg);
    exit(1);
}

int main(int argc, char * argv[])
{

     if (argc < 4)
        die("More params");

     int sockServer, sockClient, port;
     char buffer[BUFFER_SIZE];
    
     struct sockaddr_in siServer;
     struct sockaddr_in siClient;
     int n; 
 
     // preiau setarile din parametrii liniei de comanda     
     port = atoi(argv[1]);
     char * accountsFile = argv[2];
     char * sharesFile = argv[3];

     
     // populare "database"
     loadAccounts(accountsFile);
     loadShares(sharesFile);
 
     sockServer = socket(PF_INET, SOCK_STREAM, 0);
     if (sockServer < 0) 
        die("Can't open socket.");
 
     // datele pt socket
     siServer.sin_family = AF_INET;
     siServer.sin_addr.s_addr = INADDR_ANY;
     siServer.sin_port = htons(port);
     
     if (bind(sockServer, (struct sockaddr *) &siServer, sizeof(struct sockaddr)) < 0) 
        die("Can't bind.");
     
     // astept conexiuni
     listen(sockServer, MAX_CLIENTS);


     fd_set readFds;
     fd_set tmpFds;
     int fdmax = 0;

     FD_ZERO(&readFds);
     FD_ZERO(&tmpFds);


     FD_SET(sockServer, &readFds);
     fdmax = sockServer;


     int  i = 0;
     unsigned int cliLength = sizeof(siClient);

     struct timeval timer;
     timer.tv_sec = 2;
     timer.tv_usec = 0;


     while (1) 
     {
        tmpFds = readFds; 
     
        if (select(fdmax + 1, &tmpFds, NULL, NULL, &timer) == -1) 
            die("Can't select.");
    
        for(i = 0; i <= fdmax; i++) 
        {
            if (FD_ISSET(i, &tmpFds)) 
            {
                if (i == sockServer) 
                {
                    sockClient = accept(sockServer, (struct sockaddr *) &siClient, &cliLength);
                    
                    // ceva nou pe socketul de listen => accept
                    if (sockClient == -1)
                        die("Can't accept.");
                    else 
                    {
                        FD_SET(sockClient, &readFds);

                        if (sockClient > fdmax) 
                            fdmax = sockClient;
                    }

                    // aici client nou conectat
                    addUser(sockClient);


                } 
                else 
                {  
                    memset(buffer, 0, BUFFER_SIZE);

                    if ((n = recv(i, buffer, sizeof(buffer), 0)) <= 0) 
                    {
                        // aici client deconectat
                        if (n == 0) 
                            printf("socket %d hung up\n", i);
                        else 
                            die("Can't recv.");
                        
                        removeUser(i);
                        close(i); 
                        FD_CLR(i, &readFds);  
                    } 
                    else 
                    {
                        // aici mesaje primite de la client 
                        printf ("SOCK(%d): %s\n", i, buffer);


                        Result * res = handleMessage(i, buffer);
                        
                        // --- download ---
                        if (buffer[0] == TYPE_DOWNLOAD)
                        {
                            if (res->resCode != MSG_IGNORE_THIS)
                                send(i, res->resStr, sizeof(res->resStr), 0);
                        }

                        // --- share / unshare / delete ---
                        if (buffer[0] == TYPE_SHARE || buffer[0] == TYPE_UNSHARE || buffer[0] == TYPE_DELETE)
                        {
                            send(i, res->resStr, sizeof(res->resStr), 0);
                        }

                        // --- upload ---
                        if (buffer[0] == TYPE_UPLOAD)
                        {
                            if (res->resCode != MSG_IGNORE_THIS)
                            {
                                send(i, res->resStr, sizeof(res->resStr), 0);
                            } 
                        }


                        // --- getfilelist ---
                        if (buffer[0] == TYPE_GETFILELIST)
                        {
                            send(i, res->resStr, sizeof(res->resStr), 0);

                            while (*(getfChunkPtr(i)) != 0)
                            {
                                free(res);
                                res = handleMessage(i, buffer);
                                send(i, res->resStr, sizeof(res->resStr), 0);
                            }
                        }
                        
                        // --- getuserlist --- 
                        if (buffer[0] == TYPE_GETUSERLIST)
                        {
                            send(i, res->resStr, sizeof(res->resStr), 0);
                            
                            
                            while (*(getuChunkPtr(i)) != 0)
                            {
                                free(res);
                                res = handleMessage(i, buffer);
                                send(i, res->resStr, sizeof(res->resStr), 0);
                            }
                        }

                        // --- login / logout ---
                        if (buffer[0] == TYPE_LOGIN || buffer[0] == TYPE_LOGOUT)
                        {
                            memset(buffer, 0, sizeof(buffer));
                            sprintf(buffer, "%d", res->resCode);
                            send(i, buffer, strlen(buffer), 0);

                        }


                        // deconectare daca incearca de prea multe ori

                        if (res->resCode == MSG_TOO_MANY_FAILED_AUTHS)
                        {
                            removeUser(i);
                            close(i);
                            FD_CLR(i, &readFds);

                        }

                        free(res);
                    }
                } 
            }
            else
            {
                if (i == 0 || i == sockServer)
                    continue;


                // n-are nimic de facut pt socketul i
                // => trimit fisier spre download daca exista
                int nOfBytes = 0;
                char * name = NULL;
                char isLastMsg = 0;
                char * content = getUploadChunk(i, &name, &nOfBytes, &isLastMsg);
                // ^preia urmatorul chunk din fisierul curent, pt download

                if (nOfBytes == 0)
                    continue;
                
                FileUploadMessage m;
                m.type = TYPE_DOWNLOAD;
                m.size = nOfBytes;
                m.lastMsg = isLastMsg; // daca e ultimul mesaj din serie, pt fisierul respectiv
               
                // formeaza structura de FileUploadMessage
                memcpy(m.name, name, strlen(name)+1);
                memcpy(m.content, content, nOfBytes);
                memcpy(buffer, &m, sizeof(m));


                send(i, buffer, sizeof(buffer), 0);

                // scurt sleep ca sa nu se aglomereze bufferul
                usleep(700);
            }
        }
     }


     close(sockServer);
   
     return 0; 
}


